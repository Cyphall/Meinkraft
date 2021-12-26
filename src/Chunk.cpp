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
	
	glm::u8vec3 pos;
	for (pos.z = 0; pos.z < 16; pos.z++)
	{
		for (pos.y = 0; pos.y < 16; pos.y++)
		{
			for (pos.x = 0; pos.x < 16; pos.x++)
			{
				BlockType xp1 = pos.x < 15 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(+1, 0, 0))->getType() : BlockType::TRANSPARENT;
				BlockType xm1 = pos.x > 0 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(-1, 0, 0))->getType() : BlockType::TRANSPARENT;
				BlockType yp1 = pos.y < 15 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(0, +1, 0))->getType() : BlockType::TRANSPARENT;
				BlockType ym1 = pos.y > 0 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(0, -1, 0))->getType() : BlockType::TRANSPARENT;
				BlockType zp1 = pos.z < 15 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(0, 0, +1))->getType() : BlockType::TRANSPARENT;
				BlockType zm1 = pos.z > 0 ? _blockContainer.getBlockDefinition(pos + glm::u8vec3(0, 0, -1))->getType() : BlockType::TRANSPARENT;
				
				_blockContainer.getBlockDefinition(pos)->genVertices(_temporaryRamBuffer, pos, xp1, xm1, yp1, ym1, zp1, zm1);
			}
		}
	}
	_state = WAITING_MESH_TRANSFER;
}

void Chunk::transferGeneratedMesh()
{
	if (_state != WAITING_MESH_TRANSFER) throw std::runtime_error("A chunk is in an invalid state.");
	
	if (!_temporaryRamBuffer.empty())
	{
		if (!_bufferSegment)
		{
			Toolbox::renderer->getChunkBufferManager().acquireAvailableSegment(_bufferSegment, _temporaryRamBuffer.size());
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
