#include "Renderer.h"

#include "Meinkraft/Rendering/RenderPass/ChunksPass.h"
#include "Meinkraft/Rendering/RenderPass/SkyboxPass.h"

Renderer::Renderer()
{
	_chunksPass = std::make_unique<ChunksPass>();
	_skyboxPass = std::make_unique<SkyboxPass>();
}

Renderer::~Renderer()
{}

void Renderer::render()
{
	_chunksPass->render(_chunkBufferManager);
	_skyboxPass->render(_chunkBufferManager);
}

ChunkBufferManager& Renderer::getChunkBufferManager()
{
	return _chunkBufferManager;
}