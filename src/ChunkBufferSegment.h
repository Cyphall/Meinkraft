#pragma once

#include <vector>
#include "GL/VertexData.h"

class ChunkBuffer;

class ChunkBufferSegment
{
public:
	// 16*16*8*6: checkerboard block configuration with the mose visible faces possible
	// 4: n° of vertices in a block face
	static inline constexpr int CHUNK_BUFFER_ELEMENT_COUNT = 16*16*8*6*4;
	static inline constexpr int CHUNK_BUFFER_SIZE = CHUNK_BUFFER_ELEMENT_COUNT*sizeof(VertexData);
	
	ChunkBufferSegment(ChunkBufferSegment&& other) = delete;
	ChunkBufferSegment& operator=(ChunkBufferSegment&& other) = delete;
	ChunkBufferSegment(const ChunkBufferSegment& other) = delete;
	ChunkBufferSegment& operator=(const ChunkBufferSegment& other) = delete;
	
	~ChunkBufferSegment();
	
	void setData(const std::vector<VertexData>& data);
	
	const ChunkBuffer& getBuffer() const;
	int getByteOffset() const;
	int getIndexOffset() const;
	
private:
	ChunkBuffer& _container;
	int _segmentIndex = -1;
	
	friend class ChunkBuffer;
	
	ChunkBufferSegment(ChunkBuffer& container, int segmentIndex);
};
