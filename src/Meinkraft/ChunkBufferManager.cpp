#include "ChunkBufferManager.h"

void ChunkBufferManager::acquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& segment, int requestedVertexCount)
{
	for (auto& buffer : _buffers)
	{
		if (buffer->tryAcquireAvailableSegment(segment, requestedVertexCount))
		{
			return;
		}
	}
	
	_buffers.emplace_back(new ChunkBuffer);
	_buffers.back()->tryAcquireAvailableSegment(segment, requestedVertexCount);
}

const std::vector<std::unique_ptr<ChunkBuffer>>& ChunkBufferManager::getBuffers() const
{
	return _buffers;
}