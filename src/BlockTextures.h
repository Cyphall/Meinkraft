#pragma once

#include "GL/Texture.h"
#include <glad/gl.h>

class BlockTextures
{
public:
	struct NativeData
	{
		GLuint64 forwardTexture;
		GLuint64 backTexture;
		GLuint64 topTexture;
		GLuint64 bottomTexture;
		GLuint64 rightTexture;
		GLuint64 leftTexture;
	};
	
	BlockTextures(const std::string& name);
	
	NativeData getData() const;

private:
	Texture _forward;
	Texture _back;
	Texture _top;
	Texture _bottom;
	Texture _right;
	Texture _left;
};