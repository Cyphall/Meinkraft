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

private:
	std::unique_ptr<ChunksPass> _chunksPass;
	std::unique_ptr<SkyboxPass> _skyboxPass;
};