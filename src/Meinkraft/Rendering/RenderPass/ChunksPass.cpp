#include "ChunksPass.h"

#include "Meinkraft/Chunk.h"
#include "Meinkraft/World.h"
#include "Meinkraft/ChunkBuffer.h"
#include "Meinkraft/ChunkBufferManager.h"
#include "Meinkraft/Toolbox.h"
#include "Meinkraft/GL/VertexData.h"
#include "Meinkraft/GL/ShaderProgramCreateInfo.h"
#include "Meinkraft/GL/ShaderProgram.h"

#include <glm/gtc/matrix_inverse.hpp>

ChunksPass::ChunksPass()
{
	// ========== SHADERS ==========
	
	ShaderProgramCreateInfo createInfo;
	createInfo.shadersFiles[GL_VERTEX_SHADER].emplace_back("forward");
	createInfo.shadersFiles[GL_FRAGMENT_SHADER].emplace_back("forward");
	
	_forwardShader = std::make_unique<ShaderProgram>(createInfo);
	
	// ========== VAOS ==========
	
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
	
	// ========== UNIFORM STORAGE BUFFERS ==========
	
	glCreateBuffers(1, &_globalUniformBuffer);
	glNamedBufferStorage(_globalUniformBuffer, sizeof(GlobalUniform), nullptr, GL_DYNAMIC_STORAGE_BIT);
	
	glCreateBuffers(1, &_chunkUniformsBuffer);
}

ChunksPass::~ChunksPass()
{
	glDeleteBuffers(1, &_chunkUniformsBuffer);
	glDeleteBuffers(1, &_globalUniformBuffer);
	glDeleteVertexArrays(1, &_blocksVao);
}

void ChunksPass::render()
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
	for (auto& buffer : Toolbox::chunkBufferManager->getBuffers())
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _globalUniformBuffer);
	
	glNamedBufferData(_chunkUniformsBuffer, maxChunkCount * sizeof(ChunkUniform), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _chunkUniformsBuffer);
	
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

bool ChunksPass::isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes)
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