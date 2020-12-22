#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include "Chunk.h"
#include <tbb/concurrent_queue.h>
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
	
	tbb::concurrent_bounded_queue<Chunk*> _chunksInitQueue;
	tbb::concurrent_bounded_queue<Chunk*> _chunkMeshGenerationQueue;
	tbb::concurrent_bounded_queue<Chunk*> _chunkMeshTransferQueue;
	
	std::thread _chunkInitThread;
	std::thread _chunkMeshGenerationThread;
	
	void handleNewChunkPos(glm::ivec3 playerChunkPos);
	
	glm::ivec3 getPlayerChunkPos();
};
