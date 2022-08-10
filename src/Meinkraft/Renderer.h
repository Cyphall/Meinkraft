#pragma once

#include "Meinkraft/ChunkBufferManager.h"
#include "Meinkraft/Texture/Block/BlockTextureManager.h"
#include "Meinkraft/Camera.h"

#include <memory>

class ShaderProgram;

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	void render();
	
	ChunkBufferManager& getChunkBufferManager();

private:
	std::unique_ptr<ShaderProgram> _forwardShader;
	GLuint _blocksVao;
	
	std::unique_ptr<ShaderProgram> _skyboxShader;
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
