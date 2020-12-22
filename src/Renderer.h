#pragma once

#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Camera.h"
#include <memory>

struct BindlessPtrNV
{
	GLuint   index;
	GLuint   reserved;
	GLuint64 address;
	GLuint64 length;
};

struct DrawArraysIndirectCommand
{
	GLuint  count;
	GLuint  instanceCount;
	GLuint  first;
	GLuint  baseInstance;
};

struct DrawArraysIndirectBindlessCommandNV
{
	DrawArraysIndirectCommand   cmd;
	BindlessPtrNV               vertexBuffers[1];
};

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

class Renderer
{
public:
	Renderer();
	
	void render();

private:
	ShaderProgram _forwardShader;
	GLuint _vao;
	
	GLuint _globalUniformBuffer;
	GLuint _chunkUniformsBuffer;
	
	GLuint _texturesBuffer;
	
	std::vector<std::unique_ptr<Texture>> _textures;
	
	void prepare();
	void renderImpl();
	void restore();
	
	bool isInFrustum(glm::dvec3 chunkCenterPos, const glm::mat4& vp, const std::array<FrustumPlane, 4>& frustumPlanes);
};
