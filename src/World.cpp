#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "World.h"
#include "Camera.h"
#include "Toolbox.h"
#include "Helper/MathHelper.h"

void chunkInitTask(tbb::concurrent_bounded_queue<Chunk*>& chunksInitQueue, tbb::concurrent_bounded_queue<Chunk*>& chunkMeshGenerationQueue)
{
	WorldGenerator worldGenerator;
	Chunk* chunk;
	
	while (true)
	{
		chunksInitQueue.pop(chunk);
		
		if (chunk == nullptr) break;
		
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->initializeBlocks(worldGenerator);
		
		chunkMeshGenerationQueue.push(chunk);
	}
	
}

void chunkMeshGenerationTask(tbb::concurrent_bounded_queue<Chunk*>& chunkMeshGenerationQueue, tbb::concurrent_bounded_queue<Chunk*>& chunkMeshTransferQueue)
{
	Chunk* chunk;
	
	while (true)
	{
		chunkMeshGenerationQueue.pop(chunk);
		
		if (chunk == nullptr) break;
		
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->generateMesh();
		
		chunkMeshTransferQueue.push(chunk);
	}
}

World::World():
_chunkInitThread(chunkInitTask, std::ref(_chunksInitQueue), std::ref(_chunkMeshGenerationQueue)),
_chunkMeshGenerationThread(chunkMeshGenerationTask, std::ref(_chunkMeshGenerationQueue), std::ref(_chunkMeshTransferQueue)),
_lastFramePlayerChunkPos(getPlayerChunkPos())
{
	handleNewChunkPos(_lastFramePlayerChunkPos);
}

World::~World()
{
	for (auto& it : _chunks)
	{
		it.second.flagForDeletion();
	}
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
	
	int transferredChunks = 0;
	Chunk* chunk;
	while (_chunkMeshTransferQueue.try_pop(chunk))
	{
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		chunk->transferGeneratedMesh();
		transferredChunks++;
		
		if (transferredChunks >= 10) break; // Prevent more than 10 chunks to be transferred every frame
	}
}

const std::unordered_map<glm::ivec3, Chunk>& World::getChunks()
{
	return _chunks;
}

void World::handleNewChunkPos(glm::ivec3 playerChunkPos)
{
	float renderDistance = 8;
	
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
			if (state == WAITING_BLOCKS_GENERATION || state == WAITING_MESH_GENERATION || state == WAITING_MESH_TRANSFER)
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
