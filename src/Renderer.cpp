#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include "Toolbox.h"
#include "GL/Uniforms.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_inverse.hpp>
#include "ChunkBuffer.h"

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
	
	glCreateBuffers(1, &_globalUniformBuffer);
	glNamedBufferStorage(_globalUniformBuffer, sizeof(GlobalUniform),  nullptr, GL_DYNAMIC_STORAGE_BIT);
	
	glCreateBuffers(1, &_chunkUniformsBuffer);
	glNamedBufferStorage(_chunkUniformsBuffer, ChunkBuffer::CHUNK_COUNT * sizeof(ChunkUniform),  nullptr, GL_DYNAMIC_STORAGE_BIT);
	
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
	
	GlobalUniform globalUniform;
	globalUniform.viewProjection = Toolbox::camera->getProjection() * Toolbox::camera->getView();
	globalUniform.viewPos = Toolbox::camera->getPosition();
	
	const std::array<FrustumPlane, 4>& frustumPlanes = Toolbox::camera->getFrustumPlanes();
	
	std::map<const ChunkBuffer*, std::vector<const Chunk*>> chunksToDraw;
	_forwardShader.bind();
	for (auto& it : Toolbox::world->getChunks())
	{
		const Chunk& chunk = it.second;
		
		if (chunk.getState() != READY) continue;
		if (!chunk.shouldBeDrawn()) continue;
		if (!isInFrustum(chunk.getPosition() * 16 + 8, globalUniform.viewProjection, frustumPlanes)) continue;
		
		const ChunkBufferSegment* bufferSegment = chunk.getBufferSegment();
		chunksToDraw[&bufferSegment->getBuffer()].push_back(&chunk);
	}
	
	glNamedBufferSubData(_globalUniformBuffer, 0, sizeof(GlobalUniform), &globalUniform);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _globalUniformBuffer);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _texturesBuffer);
	
	std::vector<ChunkUniform> chunkUniforms;
	chunkUniforms.reserve(ChunkBuffer::CHUNK_COUNT);
	std::vector<GLint> chunkOffsets;
	chunkOffsets.reserve(ChunkBuffer::CHUNK_COUNT);
	std::vector<GLsizei> chunkVerticeCounts;
	chunkVerticeCounts.reserve(ChunkBuffer::CHUNK_COUNT);
	for (auto& [chunkBuffer, chunks] : chunksToDraw)
	{
		chunkUniforms.clear();
		chunkOffsets.clear();
		chunkVerticeCounts.clear();
		for (int i = 0; i < chunks.size(); i++)
		{
			ChunkUniform& uniform = chunkUniforms.emplace_back();
			uniform.model = chunks[i]->getModel();
			uniform.normalMatrix = glm::inverseTranspose(glm::mat3(uniform.model));
			
			chunkOffsets.push_back(chunks[i]->getBufferSegment()->getIndexOffset());
			
			chunkVerticeCounts.push_back(chunks[i]->getVerticeCount());
		}
		
		glNamedBufferSubData(_chunkUniformsBuffer, 0, sizeof(ChunkUniform) * chunkUniforms.size(), chunkUniforms.data());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _chunkUniformsBuffer);
		
		glVertexArrayVertexBuffer(_vao, 0, chunkBuffer->getGLBuffer(), 0, sizeof(VertexData));
		
		glMultiDrawArrays(GL_TRIANGLES, chunkOffsets.data(), chunkVerticeCounts.data(), chunks.size());
	}
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
		if (glm::dot(chunkCenterPos - (plane.position - plane.normal * CHUNK_FRUSTUM_TEST_OFFSET), plane.normal) < 0)
		{
			return false;
		}
	}
	
	return true;
}

ChunkBufferManager& Renderer::getChunkBufferManager()
{
	return _chunkBufferManager;
}
