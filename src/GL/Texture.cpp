#include "Texture.h"
#include <stb_image.h>
#include <fmt/format.h>

Texture::Texture(const std::string& path)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &_handle);
	
	glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	GLfloat anisoCount;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisoCount);
	glTextureParameterf(_handle, GL_TEXTURE_MAX_ANISOTROPY, anisoCount);
	
	stbi_uc* data = stbi_load(fmt::format("resources/textures/{}", path).c_str(), &_size.x, &_size.y, nullptr, 3);
	glTextureStorage2D(_handle, calculateMipmapCount(_size), GL_SRGB8, _size.x, _size.y);
	glTextureSubImage2D(_handle, 0, 0, 0, _size.x, _size.y, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	
	glGenerateTextureMipmap(_handle);
	
	_bindlessHandle = glGetTextureHandleARB(_handle);
	glMakeTextureHandleResidentARB(_bindlessHandle);
}

Texture::Texture(Texture&& other):
		_size(other._size), _handle(other._handle), _bindlessHandle(other._bindlessHandle)
{
	other._handle = 0;
	other._bindlessHandle = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_handle);
}

int Texture::calculateMipmapCount(const glm::ivec2& size)
{
	return (int)glm::floor(glm::log2((float)glm::max(size.x, size.y))) + 1;
}

GLuint64 Texture::getBindlessHandle() const
{
	return _bindlessHandle;
}
