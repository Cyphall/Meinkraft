#pragma once

#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Camera.h"
#include "BlockTextures.h"
#include "ChunkBufferManager.h"
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
	
	GLuint _texturesBuffer;
	
	std::vector<BlockTextures> _textures;
	
	ChunkBufferManager _chunkBufferManager;
	
	void renderChunks();
	void renderSkybox();
	
	bool isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes);
	
	static constexpr double CHUNK_FRUSTUM_TEST_OFFSET = 13.8564064606; // sqrt(8^2 + 8^2 + 8^2)
};
