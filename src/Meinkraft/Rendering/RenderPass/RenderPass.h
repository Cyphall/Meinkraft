#pragma once

class ChunkBufferManager;

class RenderPass
{
public:
	RenderPass() = default;
	virtual ~RenderPass() = default;
	
	virtual void render(const ChunkBufferManager& chunkBufferManager) = 0;
};