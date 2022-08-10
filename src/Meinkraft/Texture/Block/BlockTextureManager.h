#pragma once

#include <map>
#include "BlockTextureId.h"
#include "BlockTexture.h"

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
