#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include "Toolbox.h"
#include "GL/NVCommands.h"
#include "GL/Uniforms.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>

Renderer::Renderer():
_forwardShader({
	{GL_VERTEX_SHADER, "forward"},
	{GL_FRAGMENT_SHADER, "forward"}
})
{
	glCreateVertexArrays(1, &_vao);
	
	glVertexArrayAttribBinding(_vao, 0, 0);
	glVertexArrayAttribIFormat(_vao, 0, 3, GL_UNSIGNED_BYTE, offsetof(VertexData, position));
	glEnableVertexArrayAttrib(_vao, 0);
	
	glVertexArrayAttribBinding(_vao, 1, 0);
	glVertexArrayAttribIFormat(_vao, 1, 2, GL_UNSIGNED_BYTE, offsetof(VertexData, uv));
	glEnableVertexArrayAttrib(_vao, 1);
	
	glVertexArrayAttribBinding(_vao, 2, 0);
	glVertexArrayAttribIFormat(_vao, 2, 1, GL_UNSIGNED_BYTE, offsetof(VertexData, face));
	glEnableVertexArrayAttrib(_vao, 2);
	
	glVertexArrayAttribBinding(_vao, 3, 0);
	glVertexArrayAttribIFormat(_vao, 3, 1, GL_UNSIGNED_BYTE, offsetof(VertexData, block));
	glEnableVertexArrayAttrib(_vao, 3);
	
	glVertexArrayVertexBuffer(_vao, 0, 0, 0, sizeof(VertexData));
	
	glCreateBuffers(1, &_globalUniformBuffer);
	glCreateBuffers(1, &_chunkUniformsBuffer);
	
	glCreateBuffers(1, &_texturesBuffer);
	
	_textures.emplace_back("stone");
	_textures.emplace_back("grass");
	_textures.emplace_back("dirt");
	_textures.emplace_back("wood");
	_textures.emplace_back("iron");
	
	std::vector<BlockTextures::NativeData> textureHandles;
	textureHandles.reserve(_textures.size());
	for (const BlockTextures& textures : _textures)
	{
		textureHandles.push_back(textures.getData());
	}
	
	glNamedBufferStorage(_texturesBuffer, textureHandles.size() * sizeof(BlockTextures::NativeData), textureHandles.data(), 0);
}

void Renderer::render()
{
	prepare();
	renderImpl();
	restore();
}

void Renderer::prepare()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glClearColor(0x87 / 255.0f, 0xCE / 255.0f, 0xFA / 255.0f, 0xFF / 255.0f);
}

void Renderer::renderImpl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(_vao);
	
	std::vector<DrawArraysIndirectBindlessCommandNV> commands;
	
	GlobalUniform globalUniform;
	globalUniform.viewProjection = Toolbox::camera->getProjection() * Toolbox::camera->getView();
	globalUniform.viewPos = Toolbox::camera->getPosition();
	std::vector<ChunkUniform> chunkUniforms;
	
	const std::array<FrustumPlane, 4>& frustumPlanes = Toolbox::camera->getFrustumPlanes();
	
	_forwardShader.bind();
	for (auto& it : Toolbox::world->getChunks())
	{
		const Chunk& chunk = it.second;
		
		if (chunk.getState() != READY) continue;
		if (chunk.getVerticeCount() == 0) continue;
		if (!isInFrustum(chunk.getPosition() * 16 + 8, globalUniform.viewProjection, frustumPlanes)) continue;
		
		ChunkUniform uniform;
		uniform.model = chunk.getModel();
		uniform.normalMatrix = glm::inverseTranspose(glm::mat3(uniform.model));
		
		chunkUniforms.push_back(uniform);
		
		DrawArraysIndirectCommand cmd;
		cmd.count = chunk.getVerticeCount();
		cmd.first = 0;
		cmd.baseInstance = 0;
		cmd.instanceCount = 1;
		
		BindlessPtrNV ptr;
		ptr.index = 0;
		ptr.address = chunk.getVboAddress();
		ptr.length = sizeof(VertexData) * chunk.getVerticeCount();
		
		DrawArraysIndirectBindlessCommandNV cmdNV;
		cmdNV.cmd = cmd;
		cmdNV.vertexBuffers[0] = ptr;
		
		commands.push_back(cmdNV);
	}
	
	glNamedBufferData(_globalUniformBuffer, sizeof(GlobalUniform), &globalUniform, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _globalUniformBuffer);
	glNamedBufferData(_chunkUniformsBuffer, sizeof(ChunkUniform) * chunkUniforms.size(), chunkUniforms.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _chunkUniformsBuffer);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _texturesBuffer);
	
	glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
	glMultiDrawArraysIndirectBindlessNV(GL_TRIANGLES, commands.data(), commands.size(), 0, 1);
	glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
}

void Renderer::restore()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 0);
}

bool Renderer::isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes)
{
	for (const FrustumPlane& plane : frustumPlanes)
	{
		if (glm::dot(chunkCenterPos - plane.position, plane.normal) < 0)
		{
			return false;
		}
	}
	
	return true;
}
