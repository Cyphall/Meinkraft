#pragma once

#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "Camera.h"
#include <memory>

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
