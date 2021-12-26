#pragma once

#include "GL/ShaderProgram.h"
#include "Texture/Block/BlockTexture.h"
#include "Camera.h"
#include "ChunkBufferManager.h"
#include "Texture/Block/BlockTextureManager.h"
#include <memory>

class Renderer
{
public:
	Renderer();
	
	void render();
	
	ChunkBufferManager& getChunkBufferManager();

private:
	ShaderProgram _forwardShader;
	GLuint _blocksVao;
	
	ShaderProgram _skyboxShader;
	GLuint _skyboxVao;
	GLuint _skyboxVbo;
	
	GLuint _globalUniformBuffer;
	GLuint _chunkUniformsBuffer;
	
	ChunkBufferManager _chunkBufferManager;
	
	BlockTextureManager _blockTextureManager;
	
	void renderChunks();
	void renderSkybox();
	
	bool isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes);
	
	static constexpr double CHUNK_FRUSTUM_TEST_OFFSET = 13.8564064606; // sqrt(8^2 + 8^2 + 8^2)
};
