#pragma once

#include <glm/glm.hpp>

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