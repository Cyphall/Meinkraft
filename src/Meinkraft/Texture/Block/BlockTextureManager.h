#pragma once

#include "Meinkraft/Texture/Block/BlockTextureId.h"
#include "Meinkraft/Texture/Block/BlockTexture.h"

#include <map>

class BlockTextureManager
{
public:
	BlockTextureManager();
	~BlockTextureManager();
	
	GLuint getBuffer() const;

private:
	std::map<BlockTextureId, BlockTexture> _textures;
	GLuint _texturesBuffer;
	
	void registerTextures();
};