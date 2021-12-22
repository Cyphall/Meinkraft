#include "Chunk.h"
#include "Toolbox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "Renderer.h"
#include "ChunkBufferManager.h"

Chunk::Chunk(glm::ivec3 position):
_position(position)
{
	_model = glm::translate(glm::mat4(1), static_cast<glm::vec3>(_position) * 16.0f);
}

void Chunk::initializeBlocks(WorldGenerator& worldGenerator)
{
	if (_state != WAITING_BLOCKS_GENERATION) throw std::runtime_error("A chunk is in an invalid state.");
	
	_blockContainer = worldGenerator.generateMountains(_position.x, _position.y, _position.z);
	
	_state = WAITING_MESH_GENERATION;
}

void Chunk::generateMesh()
{
	if (_state != WAITING_MESH_GENERATION) throw std::runtime_error("A chunk is in an invalid state.");
	
	for (int z = 0; z < 16; z++)
	{
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				BlockType block = _blockContainer.getBlock(x, y, z);
				
				if (block == AIR) continue;
				
				// x + 1
				if ((x + 1 < 16 && _blockContainer.getBlock(x+1, y, z) == AIR) || x + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 0;
					vertexTL.face = 0;
					vertexBR.face = 0;
					vertexTR.face = 0;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
				
				// x - 1
				if ((x - 1 > -1 && _blockContainer.getBlock(x-1, y, z) == AIR) || x - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 1;
					vertexTL.face = 1;
					vertexBR.face = 1;
					vertexTR.face = 1;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
				
				// y + 1
				if ((y + 1 < 16 && _blockContainer.getBlock(x, y+1, z) == AIR) || y + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 2;
					vertexTL.face = 2;
					vertexBR.face = 2;
					vertexTR.face = 2;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
				
				// y - 1
				if ((y - 1 > -1 && _blockContainer.getBlock(x, y-1, z) == AIR) || y - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 3;
					vertexTL.face = 3;
					vertexBR.face = 3;
					vertexTR.face = 3;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
				
				// z + 1
				if ((z + 1 < 16 && _blockContainer.getBlock(x, y, z+1) == AIR) || z + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 4;
					vertexTL.face = 4;
					vertexBR.face = 4;
					vertexTR.face = 4;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
				
				// z - 1
				if ((z - 1 > -1 && _blockContainer.getBlock(x, y, z-1) == AIR) || z - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					
					vertexBL.uv = glm::u8vec2(0, 0);
					vertexTL.uv = glm::u8vec2(0, 1);
					vertexBR.uv = glm::u8vec2(1, 0);
					vertexTR.uv = glm::u8vec2(1, 1);
					
					vertexBL.face = 5;
					vertexTL.face = 5;
					vertexBR.face = 5;
					vertexTR.face = 5;
					
					vertexBL.block = block;
					vertexTL.block = block;
					vertexBR.block = block;
					vertexTR.block = block;
					
					_temporaryRamBuffer.push_back(vertexBL);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexBR);
					_temporaryRamBuffer.push_back(vertexTL);
					_temporaryRamBuffer.push_back(vertexTR);
				}
			}
		}
	}
	_verticeCount = _temporaryRamBuffer.size();
	_state = WAITING_MESH_TRANSFER;
}

void Chunk::transferGeneratedMesh()
{
	if (_state != WAITING_MESH_TRANSFER) throw std::runtime_error("A chunk is in an invalid state.");
	
	if (_verticeCount > 0)
	{
		if (!_bufferSegment)
		{
			Toolbox::renderer->getChunkBufferManager().acquireAvailableSegment(_bufferSegment);
		}
		
		_bufferSegment->setData(_temporaryRamBuffer);
		_temporaryRamBuffer = std::vector<VertexData>();
	}
	
	_state = READY;
}

ChunkState Chunk::getState() const
{
	return _state;
}

glm::ivec3 Chunk::getPosition() const
{
	return _position;
}

glm::mat4 Chunk::getModel() const
{
	return _model;
}

void Chunk::flagForDeletion()
{
	_deletionFlag = true;
}

bool Chunk::isFlaggedForDeletion() const
{
	return _deletionFlag;
}

void Chunk::confirmDeletionFlag()
{
	_safelyDestroyableFlag = true;
}

bool Chunk::isSafelyDestroyable() const
{
	return _safelyDestroyableFlag;
}

BlockContainer& Chunk::getBlockContainer()
{
	return _blockContainer;
}

const ChunkBufferSegment* Chunk::getBufferSegment() const
{
	return _bufferSegment.get();
}

bool Chunk::shouldBeDrawn() const
{
	return static_cast<bool>(_bufferSegment);
}

size_t Chunk::getVerticeCount() const
{
	return _verticeCount;
}
