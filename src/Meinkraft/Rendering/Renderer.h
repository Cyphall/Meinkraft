#pragma once

#include "Meinkraft/ChunkBufferManager.h"
#include "Meinkraft/Texture/Block/BlockTextureManager.h"

#include <memory>

class ShaderProgram;
class ChunksPass;
class SkyboxPass;

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	void render();
	
	ChunkBufferManager& getChunkBufferManager();

private:
	ChunkBufferManager _chunkBufferManager;
	BlockTextureManager _blockTextureManager;
	
	std::unique_ptr<ChunksPass> _chunksPass;
	std::unique_ptr<SkyboxPass> _skyboxPass;
};