#include "MainThreadChunkTaskQueue.h"

#include <format>
#include <stdexcept>
#include <magic_enum.hpp>

void MainThreadChunkTaskQueue::push(Chunk* chunk)
{
	std::scoped_lock lock(_mutex);
	
	switch (chunk->getState())
	{
		case ChunkState::WAITING_BUFFER_SEGMENT_RESERVATION:
			_bufferSegmentReservationQueue.push(chunk);
			break;
		case ChunkState::WAITING_MESH_UPLOAD:
			_meshUploadQueue.push(chunk);
			break;
		default:
			throw std::logic_error(std::format("This chunk state is not supposed to be processed by the main thread: {}", magic_enum::enum_name(chunk->getState())));
	}
}

bool MainThreadChunkTaskQueue::tryPop(Chunk*& chunk)
{
	std::scoped_lock lock(_mutex);
	
	if (!_meshUploadQueue.empty())
	{
		chunk = _meshUploadQueue.front();
		_meshUploadQueue.pop();
		
		return true;
	}
	if (!_bufferSegmentReservationQueue.empty())
	{
		chunk = _bufferSegmentReservationQueue.front();
		_bufferSegmentReservationQueue.pop();
		
		return true;
	}
	
	return false;
}