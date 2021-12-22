#pragma once

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <string>

class Texture
{
public:
	explicit Texture(const std::string& path);
	Texture(const Texture& other) = delete;
	Texture(Texture&& other);
	
	~Texture();
	
	GLuint64 getBindlessHandle() const;

private:
	glm::ivec2 _size;
	GLuint64 _bindlessHandle;
	
	GLuint _handle;
	
	static int calculateMipmapCount(const glm::ivec2& size);
};


