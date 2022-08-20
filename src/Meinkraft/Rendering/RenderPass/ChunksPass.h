#pragma once

#include "Meinkraft/Rendering/RenderPass/RenderPass.h"
#include "Meinkraft/Camera.h"

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <memory>

class ShaderProgram;

class ChunksPass : public RenderPass
{
public:
	ChunksPass();
	~ChunksPass() override;
	
	void render(const ChunkBufferManager& chunkBufferManager) override;
	
private:
	struct GlobalUniform
	{
		glm::mat4 viewProjection;
		glm::vec3 viewPos;
		float padding;
	};
	
	struct ChunkUniform
	{
		glm::mat4 model;
		glm::mat4 normalMatrix;
	};
	
	std::unique_ptr<ShaderProgram> _forwardShader;
	GLuint _blocksVao;
	
	GLuint _globalUniformBuffer;
	GLuint _chunkUniformsBuffer;
	
	bool isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes);
	
	static constexpr double CHUNK_FRUSTUM_TEST_OFFSET = 13.8564064606; // sqrt(8^2 + 8^2 + 8^2)
};