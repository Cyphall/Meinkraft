#include "Chunk.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

Chunk::Chunk(glm::ivec3 position):
_position(position)
{
	_model = glm::translate(glm::mat4(1), static_cast<glm::vec3>(_position) * 16.0f);
}

Chunk::~Chunk()
{
	if (_vbo)
	{
		glMakeNamedBufferNonResidentNV(_vbo);
		glDeleteBuffers(1, &_vbo);
	}
}

void Chunk::initializeBlocks(WorldGenerator& worldGenerator)
{
	if (_state != WAITING_BLOCKS_GENERATION) throw std::runtime_error("A chunk is in an invalid state.");
	
	_blocks = worldGenerator.generateMountains(_position.x, _position.y, _position.z);
	
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
				BlockType block = getBlock(x, y, z);
				
				if (block == AIR) continue;
				
				// x + 1
				if ((x + 1 < 16 && getBlock(x+1, y, z) == AIR) || x + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					
					glm::u8vec2 uvOffset(3, 2);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
				if ((x - 1 > -1 && getBlock(x-1, y, z) == AIR) || x - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					
					glm::u8vec2 uvOffset(1, 2);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
				if ((y + 1 < 16 && getBlock(x, y+1, z) == AIR) || y + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					
					glm::u8vec2 uvOffset(1, 3);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
				if ((y - 1 > -1 && getBlock(x, y-1, z) == AIR) || y - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					
					glm::u8vec2 uvOffset(1, 1);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
				if ((z + 1 < 16 && getBlock(x, y, z+1) == AIR) || z + 1 == 16)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 0, y+ 0, z+ 1);
					vertexTL.position = glm::u8vec3(x+ 0, y+ 1, z+ 1);
					vertexBR.position = glm::u8vec3(x+ 1, y+ 0, z+ 1);
					vertexTR.position = glm::u8vec3(x+ 1, y+ 1, z+ 1);
					
					glm::u8vec2 uvOffset(2, 2);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
				if ((z - 1 > -1 && getBlock(x, y, z-1) == AIR) || z - 1 == -1)
				{
					VertexData vertexBL;
					VertexData vertexTL;
					VertexData vertexBR;
					VertexData vertexTR;
					
					vertexBL.position = glm::u8vec3(x+ 1, y+ 0, z+ 0);
					vertexTL.position = glm::u8vec3(x+ 1, y+ 1, z+ 0);
					vertexBR.position = glm::u8vec3(x+ 0, y+ 0, z+ 0);
					vertexTR.position = glm::u8vec3(x+ 0, y+ 1, z+ 0);
					
					glm::u8vec2 uvOffset(0, 2);
					
					vertexBL.uv = uvOffset + glm::u8vec2(0, 0);
					vertexTL.uv = uvOffset + glm::u8vec2(0, 1);
					vertexBR.uv = uvOffset + glm::u8vec2(1, 0);
					vertexTR.uv = uvOffset + glm::u8vec2(1, 1);
					
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
	_state = WAITING_MESH_TRANSFER;
}

void Chunk::transferGeneratedMesh()
{
	if (_state != WAITING_MESH_TRANSFER) throw std::runtime_error("A chunk is in an invalid state.");
	
	if (_vbo)
	{
		glMakeNamedBufferNonResidentNV(_vbo);
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
		_vboAddress = 0;
	}
	
	if (_temporaryRamBuffer.size() > 0)
	{
		glCreateBuffers(1, &_vbo);
		glNamedBufferStorage(_vbo, _temporaryRamBuffer.size() * sizeof(VertexData), _temporaryRamBuffer.data(), 0);
		glGetNamedBufferParameterui64vNV(_vbo, GL_BUFFER_GPU_ADDRESS_NV, &_vboAddress);
		glMakeNamedBufferResidentNV(_vbo, GL_READ_ONLY);
	}
	
	_verticeCount = _temporaryRamBuffer.size();
	_temporaryRamBuffer = std::vector<VertexData>();
	
	_state = READY;
}

BlockType Chunk::getBlock(int x, int y, int z) const
{
	return _blocks[x + y * 16 + z * 256];
}

void Chunk::setBlock(int x, int y, int z, BlockType block)
{
	_blocks[x + y * 16 + z * 256] = block;
}

ChunkState Chunk::getState() const
{
	return _state;
}

glm::ivec3 Chunk::getPosition() const
{
	return _position;
}

GLuint64 Chunk::getVboAddress() const
{
	return _vboAddress;
}

size_t Chunk::getVerticeCount() const
{
	return _verticeCount;
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
