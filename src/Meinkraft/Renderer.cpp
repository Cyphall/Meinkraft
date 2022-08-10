#include "Renderer.h"

#include "Meinkraft/World.h"
#include "Meinkraft/Toolbox.h"
#include "Meinkraft/GL/Uniforms.h"
#include "Meinkraft/ChunkBuffer.h"
#include "Meinkraft/GL/ShaderProgram.h"
#include "Meinkraft/GL/ShaderProgramCreateInfo.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_inverse.hpp>

Renderer::Renderer()
{
	// ========== SHADERS ==========
	
	{
		ShaderProgramCreateInfo createInfo;
		createInfo.shadersFiles[GL_VERTEX_SHADER].emplace_back("forward");
		createInfo.shadersFiles[GL_FRAGMENT_SHADER].emplace_back("forward");
		
		_forwardShader = std::make_unique<ShaderProgram>(createInfo);
	}
	
	{
		ShaderProgramCreateInfo createInfo;
		createInfo.shadersFiles[GL_VERTEX_SHADER].emplace_back("skybox");
		createInfo.shadersFiles[GL_FRAGMENT_SHADER].emplace_back("skybox");
		
		_skyboxShader = std::make_unique<ShaderProgram>(createInfo);
	}
	
	// ========== MAIN VAO ==========
	
	glCreateVertexArrays(1, &_blocksVao);
	
	glVertexArrayAttribBinding(_blocksVao, 0, 0);
	glVertexArrayAttribIFormat(_blocksVao, 0, 3, GL_UNSIGNED_BYTE, offsetof(VertexData, position));
	glEnableVertexArrayAttrib(_blocksVao, 0);
	
	glVertexArrayAttribBinding(_blocksVao, 1, 0);
	glVertexArrayAttribIFormat(_blocksVao, 1, 2, GL_UNSIGNED_BYTE, offsetof(VertexData, uv));
	glEnableVertexArrayAttrib(_blocksVao, 1);
	
	glVertexArrayAttribBinding(_blocksVao, 2, 0);
	glVertexArrayAttribIFormat(_blocksVao, 2, 1, GL_UNSIGNED_BYTE, offsetof(VertexData, face));
	glEnableVertexArrayAttrib(_blocksVao, 2);
	
	glVertexArrayAttribBinding(_blocksVao, 3, 0);
	glVertexArrayAttribIFormat(_blocksVao, 3, 1, GL_UNSIGNED_BYTE, offsetof(VertexData, texture));
	glEnableVertexArrayAttrib(_blocksVao, 3);
	
	// ========== SKYBOX VAO ==========
	
	glCreateVertexArrays(1, &_skyboxVao);
	
	glVertexArrayAttribBinding(_skyboxVao, 0, 0);
	glVertexArrayAttribIFormat(_skyboxVao, 0, 3, GL_BYTE, 0);
	glEnableVertexArrayAttrib(_skyboxVao, 0);
	
	// ========== SKYBOX VBO ==========
	
	std::vector<glm::i8vec3> skyboxData = {
		{-1,  1, -1},
		{-1, -1, -1},
		{ 1, -1, -1},
		{ 1, -1, -1},
		{ 1,  1, -1},
		{-1,  1, -1},
		
		{-1, -1,  1},
		{-1, -1, -1},
		{-1,  1, -1},
		{-1,  1, -1},
		{-1,  1,  1},
		{-1, -1,  1},
		
		{ 1, -1, -1},
		{ 1, -1,  1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{ 1,  1, -1},
		{ 1, -1, -1},
		
		{-1, -1,  1},
		{-1,  1,  1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{ 1, -1,  1},
		{-1, -1,  1},
		
		{-1,  1, -1},
		{ 1,  1, -1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{-1,  1,  1},
		{-1,  1, -1},
		
		{-1, -1, -1},
		{-1, -1,  1},
		{ 1, -1, -1},
		{ 1, -1, -1},
		{-1, -1,  1},
		{ 1, -1,  1}
	};
	
	glCreateBuffers(1, &_skyboxVbo);
	glNamedBufferStorage(_skyboxVbo, skyboxData.size() * sizeof(glm::i8vec3), skyboxData.data(), 0);
	
	glVertexArrayVertexBuffer(_skyboxVao, 0, _skyboxVbo, 0, sizeof(glm::i8vec3));
	
	// ========== GLOBAL UNIFORMS SSBO ==========
	
	glCreateBuffers(1, &_globalUniformBuffer);
	glNamedBufferStorage(_globalUniformBuffer, sizeof(GlobalUniform), nullptr, GL_DYNAMIC_STORAGE_BIT);
	
	// ========== CHUNK UNIFORMS SSBO ==========
	
	glCreateBuffers(1, &_chunkUniformsBuffer);
}

Renderer::~Renderer()
{}

void Renderer::render()
{
	renderChunks();
	renderSkybox();
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

void Renderer::renderChunks()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(_blocksVao);
	
	GlobalUniform globalUniform;
	globalUniform.viewProjection = Toolbox::camera->getProjection() * Toolbox::camera->getView();
	globalUniform.viewPos = Toolbox::camera->getPosition();
	
	const std::array<FrustumPlane, 4>& frustumPlanes = Toolbox::camera->getFrustumPlanes();
	
	std::map<const ChunkBuffer*, std::vector<const Chunk*>> chunksToDraw;
	int maxChunkCount = 0;
	for (auto& buffer : _chunkBufferManager.getBuffers())
	{
		int chunkCount = buffer->getActiveSegmentCount();
		chunksToDraw[buffer.get()].reserve(chunkCount);
		maxChunkCount = std::max(maxChunkCount, chunkCount);
	}
	
	for (auto& it : Toolbox::world->getChunks())
	{
		const Chunk& chunk = it.second;
		
		if (chunk.getState() != ChunkState::READY) continue;
		if (!chunk.shouldBeDrawn()) continue;
		if (!isInFrustum(chunk.getPosition() * 16 + 8, globalUniform.viewProjection, frustumPlanes)) continue;
		
		const ChunkBufferSegment* bufferSegment = chunk.getBufferSegment();
		chunksToDraw[&bufferSegment->getBuffer()].push_back(&chunk);
	}
	
	glNamedBufferSubData(_globalUniformBuffer, 0, sizeof(GlobalUniform), &globalUniform);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _globalUniformBuffer);
	
	glNamedBufferData(_chunkUniformsBuffer, maxChunkCount * sizeof(ChunkUniform), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _chunkUniformsBuffer);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _blockTextureManager.getBuffer());
	
	_forwardShader->bind();
	
	std::vector<ChunkUniform> chunkUniforms;
	std::vector<GLint> chunkStartIndices;
	std::vector<GLsizei> chunkVerticeCounts;
	for (auto& [chunkBuffer, chunks] : chunksToDraw)
	{
		chunkUniforms.clear();
		chunkUniforms.reserve(chunks.size());
		chunkStartIndices.clear();
		chunkStartIndices.reserve(chunks.size());
		chunkVerticeCounts.clear();
		chunkVerticeCounts.reserve(chunks.size());
		
		for (int i = 0; i < chunks.size(); i++)
		{
			ChunkUniform& uniform = chunkUniforms.emplace_back();
			uniform.model = chunks[i]->getModel();
			uniform.normalMatrix = glm::inverseTranspose(glm::mat3(uniform.model));
			
			const ChunkBufferSegment* segment = chunks[i]->getBufferSegment();
			chunkStartIndices.push_back(segment->getStartIndex());
			chunkVerticeCounts.push_back(segment->getVertexCount());
		}
		
		glNamedBufferSubData(_chunkUniformsBuffer, 0, chunkUniforms.size() * sizeof(ChunkUniform), chunkUniforms.data());
		
		glVertexArrayVertexBuffer(_blocksVao, 0, chunkBuffer->getGLBuffer(), 0, sizeof(VertexData));
		
		glMultiDrawArrays(GL_TRIANGLES, chunkStartIndices.data(), chunkVerticeCounts.data(), chunks.size());
	}
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void Renderer::renderSkybox()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	
	glBindVertexArray(_skyboxVao);
	
	_skyboxShader->bind();
	
	glm::mat4 mvp =
		Toolbox::camera->getProjection() *
		glm::mat4(glm::mat3(Toolbox::camera->getView()));
	
	_skyboxShader->setUniform("u_mvp", mvp);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}
