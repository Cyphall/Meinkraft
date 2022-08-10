#pragma once

#include "Meinkraft/Chunk.h"

#include <queue>
#include <mutex>
#include <semaphore>

class ConcurrentChunkTaskQueue
{
public:
	void push(Chunk* chunk);
	Chunk* pop();
	
	void stop(int threadCount);
	
private:
	std::queue<Chunk*> _blocksGenerationQueue;
	std::queue<Chunk*> _meshGenerationQueue;
	
	std::mutex _mutex;
	
	std::atomic_bool _stopSignal;
	
	std::counting_semaphore<> _semaphore = std::counting_semaphore<>(0);
};