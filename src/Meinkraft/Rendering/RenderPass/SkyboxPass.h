#pragma once

#include "Meinkraft/Rendering/RenderPass/RenderPass.h"

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <memory>

class ShaderProgram;

class SkyboxPass : public RenderPass
{
public:
	SkyboxPass();
	~SkyboxPass() override;
	
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
	
	std::unique_ptr<ShaderProgram> _skyboxShader;
	GLuint _skyboxVao;
	GLuint _skyboxVbo;
};