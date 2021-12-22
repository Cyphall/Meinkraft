#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/gl.h>
#include <atomic>
#include <memory>
#include "BlockType.h"
#include "WorldGenerator.h"
#include "BlockContainer.h"
#include "GL/VertexData.h"
#include "ChunkBufferSegment.h"

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
	
	void initializeBlocks(WorldGenerator& worldGenerator);
	void generateMesh();
	
	void transferGeneratedMesh();
	
	glm::ivec3 getPosition() const;
	
	BlockContainer& getBlockContainer();
	
	ChunkState getState() const;
	glm::mat4 getModel() const;
	
	void flagForDeletion();
	bool isFlaggedForDeletion() const;
	
	void confirmDeletionFlag();
	bool isSafelyDestroyable() const;
	
	bool shouldBeDrawn() const;
	
	const ChunkBufferSegment* getBufferSegment() const;
 
private:
	glm::ivec3 _position;
	glm::mat4 _model;
	
	BlockContainer _blockContainer;
	
	std::vector<VertexData> _temporaryRamBuffer;
	std::unique_ptr<ChunkBufferSegment> _bufferSegment;
	
	std::atomic<ChunkState> _state = WAITING_BLOCKS_GENERATION;
	std::atomic<bool> _deletionFlag;
	std::atomic<bool> _safelyDestroyableFlag; // this flags certifies this chunk is not in some queue waiting to be processed
};
