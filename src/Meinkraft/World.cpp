#include "World.h"
#include "Meinkraft/Camera.h"
#include "Meinkraft/Toolbox.h"
#include "Meinkraft/Helper/MathHelper.h"
#include "Meinkraft/Helper/SpecHelper.h"

#include <glm/gtx/string_cast.hpp>
#include <format>
#include <iostream>
#include <magic_enum.hpp>

static void dispatchChunkToNextTask(Chunk* chunk, ConcurrentChunkTaskQueue& threadPoolQueue, MainThreadChunkTaskQueue& mainThreadQueue)
{
	switch (chunk->getState())
	{
		case ChunkState::WAITING_BLOCKS_GENERATION:
			threadPoolQueue.push(chunk);
			break;
		case ChunkState::WAITING_MESH_GENERATION:
			threadPoolQueue.push(chunk);
			break;
		case ChunkState::WAITING_BUFFER_SEGMENT_RESERVATION:
			mainThreadQueue.push(chunk);
			break;
		case ChunkState::WAITING_MESH_UPLOAD:
			mainThreadQueue.push(chunk);
			break;
		case ChunkState::READY:
			break;
	}
}

void threadTask(ConcurrentChunkTaskQueue& threadPoolQueue, MainThreadChunkTaskQueue& mainThreadQueue)
{
	WorldGenerator worldGenerator;
	
	while (true)
	{
		Chunk* chunk = threadPoolQueue.pop();
		
		if (chunk == nullptr)
		{
			break;
		}
		
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		switch (chunk->getState())
		{
			case ChunkState::WAITING_BLOCKS_GENERATION:
				chunk->initializeBlocks(worldGenerator);
				break;
			case ChunkState::WAITING_MESH_GENERATION:
				chunk->generateMesh();
				break;
			default:
				throw std::logic_error("This exception is not supposed to be reachable.");
		}
		
		dispatchChunkToNextTask(chunk, threadPoolQueue, mainThreadQueue);
	}
}

World::World():
_lastFramePlayerChunkPos(getPlayerChunkPos())
{
	int threadCount = std::max(SpecHelper::getPhysicalCoreCount() - 1, 1);
	for (int i = 0; i < threadCount; i++)
	{
		_threads.emplace_back(threadTask, std::ref(_threadPoolQueue), std::ref(_mainThreadQueue));
	}
	
	handleNewChunkPos(_lastFramePlayerChunkPos);
}

World::~World()
{
	_threadPoolQueue.stop(_threads.size());
	
	for (int i = 0; i < _threads.size(); i++)
	{
		_threads[i].join();
	}
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
	while (_mainThreadQueue.tryPop(chunk))
	{
		if (chunk->isFlaggedForDeletion())
		{
			chunk->confirmDeletionFlag();
			continue;
		}
		
		switch (chunk->getState())
		{
			case ChunkState::WAITING_BUFFER_SEGMENT_RESERVATION:
				chunk->reserveBufferSegment();
				break;
			case ChunkState::WAITING_MESH_UPLOAD:
				chunk->uploadMesh();
				break;
			default:
				throw std::logic_error(std::format("This chunk state is not supposed to be processed by the main thread: {}", magic_enum::enum_name(chunk->getState())));
		}
		
		dispatchChunkToNextTask(chunk, _threadPoolQueue, _mainThreadQueue);
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
						dispatchChunkToNextTask(&pair.first->second, _threadPoolQueue, _mainThreadQueue);
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