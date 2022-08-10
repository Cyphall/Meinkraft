#include "ConcurrentChunkTaskQueue.h"

#include <format>
#include <stdexcept>
#include <magic_enum.hpp>

void ConcurrentChunkTaskQueue::push(Chunk* chunk)
{
	_mutex.lock();
	
	switch (chunk->getState())
	{
		case ChunkState::WAITING_BLOCKS_GENERATION:
			_blocksGenerationQueue.push(chunk);
			break;
		case ChunkState::WAITING_MESH_GENERATION:
			_meshGenerationQueue.push(chunk);
			break;
		default:
			throw std::logic_error(std::format("This chunk state is not supposed to be processed by the off-threads: {}", magic_enum::enum_name(chunk->getState())));
	}
	
	_mutex.unlock();
	
	_semaphore.release();
}

void ConcurrentChunkTaskQueue::stop(int threadCount)
{
	_stopSignal = true;
	_semaphore.release(threadCount);
}

Chunk* ConcurrentChunkTaskQueue::pop()
{
	_semaphore.acquire();
	
	Chunk* chunk = nullptr;
	
	if (!_stopSignal)
	{
		_mutex.lock();
		
		if (!_meshGenerationQueue.empty())
		{
			chunk = _meshGenerationQueue.front();
			_meshGenerationQueue.pop();
		}
		else if (!_blocksGenerationQueue.empty())
		{
			chunk = _blocksGenerationQueue.front();
			_blocksGenerationQueue.pop();
		}
		else
		{
			throw std::logic_error("A thread was woken up with no task.");
		}
		
		_mutex.unlock();
	}
	
	return chunk;
}