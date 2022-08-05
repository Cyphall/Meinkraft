#pragma once

#include <queue>
#include <mutex>
#include <semaphore>

#include "Chunk.h"

class MainThreadChunkTaskQueue
{
public:
	void push(Chunk* chunk);
	bool tryPop(Chunk*& chunk);
	
private:
	std::queue<Chunk*> _bufferSegmentReservationQueue;
	std::queue<Chunk*> _meshUploadQueue;
	
	std::mutex _mutex;
};