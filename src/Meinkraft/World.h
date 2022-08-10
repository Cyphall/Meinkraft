#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include "Chunk.h"
#include "ConcurrentChunkTaskQueue.h"
#include "MainThreadChunkTaskQueue.h"
#include <thread>

class World
{
public:
	World();
	~World();
	
	void update();
	
	const std::unordered_map<glm::ivec3, Chunk>& getChunks();
	
private:
	std::unordered_map<glm::ivec3, Chunk> _chunks;
	
	glm::ivec3 _lastFramePlayerChunkPos;
	
	ConcurrentChunkTaskQueue _threadPoolQueue;
	MainThreadChunkTaskQueue _mainThreadQueue;

	std::vector<std::thread> _threads;
	
	void handleNewChunkPos(glm::ivec3 playerChunkPos);
	
	glm::ivec3 getPlayerChunkPos();
};