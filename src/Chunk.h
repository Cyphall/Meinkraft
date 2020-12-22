#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <atomic>
#include "BlockType.h"
#include "WorldGenerator.h"

/* faces:
0: positive x
1: negative x
2: positive y
3: negative y
4: positive z
5: negative z
 */
struct VertexData
{
	glm::u8vec3 position; // xxxxxxxx, yyyyyyyy, zzzzzzzz
	glm::u8vec2 uv;       // uuuuuuuu, vvvvvvvv
	uint8_t face;         // ffffffff
	uint8_t block;        // bbbbbbbb
};

enum ChunkState
{
	WAITING_BLOCKS_GENERATION,
	WAITING_MESH_GENERATION,
	WAITING_MESH_TRANSFER,
	READY
};

class Chunk
{
public:
	Chunk(glm::ivec3 position);
	~Chunk();
	
	void initializeBlocks(WorldGenerator& worldGenerator);
	void generateMesh();
	
	void transferGeneratedMesh();
	
	glm::ivec3 getPosition() const;
	
	BlockType getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, BlockType block);
	
	ChunkState getState() const;
	GLuint64 getVboAddress() const;
	size_t getVerticeCount() const;
	glm::mat4 getModel() const;
	
	void flagForDeletion();
	bool isFlaggedForDeletion() const;
	
	void confirmDeletionFlag();
	bool isSafelyDestroyable() const;
 
private:
	glm::ivec3 _position;
	glm::mat4 _model;
	
	std::vector<BlockType> _blocks;
	std::vector<VertexData> _temporaryRamBuffer;
	size_t _verticeCount = 0;
	
	std::atomic<ChunkState> _state = WAITING_BLOCKS_GENERATION;
	std::atomic<bool> _deletionFlag;
	std::atomic<bool> _safelyDestroyableFlag; // this flags certify this chunk is not in some queue waiting to be processed
	
	GLuint _vbo = 0;
	GLuint64 _vboAddress = 0;
};
