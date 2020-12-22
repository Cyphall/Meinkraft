#pragma once

#include <map>
#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

class ShaderProgram
{
public:
	explicit ShaderProgram(const std::map<GLenum, std::string>& createInfo);
	~ShaderProgram();
	
	void bind();
	
	void setUniform(const char* name, const float* data, size_t count = 1);
	void setUniform(const char* name, const glm::vec2* data, size_t count = 1);
	void setUniform(const char* name, const glm::vec3* data, size_t count = 1);
	void setUniform(const char* name, const glm::vec4* data, size_t count = 1);
	
	void setUniform(const char* name, const int* data, size_t count = 1);
	void setUniform(const char* name, const glm::ivec2* data, size_t count = 1);
	void setUniform(const char* name, const glm::ivec3* data, size_t count = 1);
	void setUniform(const char* name, const glm::ivec4* data, size_t count = 1);
	
	void setUniform(const char* name, const bool* data, size_t count = 1);
	void setUniform(const char* name, const glm::bvec2* data, size_t count = 1);
	void setUniform(const char* name, const glm::bvec3* data, size_t count = 1);
	void setUniform(const char* name, const glm::bvec4* data, size_t count = 1);
	
	void setUniform(const char* name, const glm::mat2* data, size_t count = 1);
	void setUniform(const char* name, const glm::mat3* data, size_t count = 1);
	void setUniform(const char* name, const glm::mat4* data, size_t count = 1);
	
	void setUniform(const char* name, const GLuint64* data);

private:
	GLuint _handle;
	
	std::map<std::string, int> _uniforms;
	static GLuint loadShader(GLenum type, const std::string& file);
	
	int getUniformLocation(const char* name);
};


