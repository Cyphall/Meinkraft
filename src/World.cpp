#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "World.h"
#include "Camera.h"
#include "Toolbox.h"
#include "Helper/MathHelper.h"

void chunkInitTask(tbb::concurrent_bounded_queue<Chunk*>& chunksInitQueue, tbb::concurrent_bounded_queue<Chunk*>& chunkMeshGenerationQueue, const std::atomic<bool>& threadsStopSignal)
{
	WorldGenerator worldGenerator;
	Chunk* chunk;
	
	while (true)
	{
		chunksInitQueue.pop(chunk);
		
		if (threadsStopSignal)
			break;
		
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->initializeBlocks(worldGenerator);
		
		chunkMeshGenerationQueue.push(chunk);
	}
	
}

void chunkMeshGenerationTask(tbb::concurrent_bounded_queue<Chunk*>& chunkMeshGenerationQueue, tbb::concurrent_bounded_queue<Chunk*>& chunkMeshBufferSegmentReservationQueue, const std::atomic<bool>& threadsStopSignal)
{
	Chunk* chunk;
	
	while (true)
	{
		chunkMeshGenerationQueue.pop(chunk);
		
		if (threadsStopSignal)
			break;
		
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->generateMesh();
		
		if (chunk->getState() == ChunkState::WAITING_BUFFER_SEGMENT_RESERVATION)
		{
			chunkMeshBufferSegmentReservationQueue.push(chunk);
		}
	}
}

World::World():
_chunkInitThread(chunkInitTask, std::ref(_chunksInitQueue), std::ref(_chunkMeshGenerationQueue), std::ref(_threadsStopSignal)),
_chunkMeshGenerationThread(chunkMeshGenerationTask, std::ref(_chunkMeshGenerationQueue), std::ref(_chunkMeshBufferSegmentReservationQueue), std::ref(_threadsStopSignal)),
_lastFramePlayerChunkPos(getPlayerChunkPos())
{
	handleNewChunkPos(_lastFramePlayerChunkPos);
}

World::~World()
{
	_threadsStopSignal = true;
	
	_chunksInitQueue.push(nullptr);
	_chunkMeshGenerationQueue.push(nullptr);
	
	if (_chunkInitThread.joinable())
		_chunkInitThread.join();
	if (_chunkMeshGenerationThread.joinable())
		_chunkMeshGenerationThread.join();
}

void World::update()
{
	glm::ivec3 playerChunkPos = getPlayerChunkPos();
	
	if (playerChunkPos != _lastFramePlayerChunkPos)
	{
		_lastFramePlayerChunkPos = playerChunkPos;
		handleNewChunkPos(playerChunkPos);
	}
	
	Chunk* chunk;
	while (_chunkMeshBufferSegmentReservationQueue.try_pop(chunk))
	{
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->reserveBufferSegment();
		_chunkMeshUploadQueue.push(chunk);
	}
	
	int uploadedChunks = 0;
	while (_chunkMeshUploadQueue.try_pop(chunk))
	{
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->uploadMesh();
		uploadedChunks++;
		
		if (uploadedChunks >= 50) break; // Prevent more than N chunks to be uploaded every frame
	}
}

const std::unordered_map<glm::ivec3, Chunk>& World::getChunks()
{
	return _chunks;
}

void World::handleNewChunkPos(glm::ivec3 playerChunkPos)
{
	float renderDistance = 16;
	
	for (auto i = _chunks.begin(); i != _chunks.end();)
	{
		glm::ivec3 relativeChunkPos = i->first - playerChunkPos;
		Chunk* chunk = &i->second;
		
		if (chunk->isFlaggedForDeletion() && chunk->isSafelyDestroyable())
		{
			i = _chunks.erase(i);
			continue;
		}
		
		if (!MathHelper::between(relativeChunkPos.x, -renderDistance, renderDistance) ||
		    !MathHelper::between(relativeChunkPos.y, -renderDistance, renderDistance) ||
		    !MathHelper::between(relativeChunkPos.z, -renderDistance, renderDistance))
		{
			ChunkState state = chunk->getState();
			if (state != ChunkState::READY)
			{
				chunk->flagForDeletion();
				i++;
				continue;
			}
			
			i = _chunks.erase(i);
		}
		else
		{
			i++;
		}
	}
	
	for (int d = 0; d <= renderDistance; d++)
	{
		for (int x = -d; x <= d; x++)
		{
			for (int y = -d; y <= d; y++)
			{
				for (int z = -d; z <= d; z++)
				{
					if (d != 0 && !(x%d == 0 || y%d == 0 || z%d == 0)) continue;
					
					glm::ivec3 relativeChunkPos = playerChunkPos + glm::ivec3(x, y, z);
					
					auto pair = _chunks.try_emplace(relativeChunkPos, relativeChunkPos);
					if (pair.second)
					{
						_chunksInitQueue.push(&pair.first->second);
					}
				}
			}
		}
	}
}

glm::ivec3 World::getPlayerChunkPos()
{
	return glm::floor(Toolbox::camera->getPosition() / 16.0);
}
