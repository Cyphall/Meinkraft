#include "ChunkBufferManager.h"
#include "ChunkBuffer.h"

void ChunkBufferManager::acquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& segment)
{
	for (auto& buffer : _buffers)
	{
		if (buffer->tryAcquireAvailableSegment(segment))
		{
			return;
		}
	}
	
	_buffers.emplace_back(new ChunkBuffer);
	_buffers.back()->tryAcquireAvailableSegment(segment);
}
