#pragma once

#include "Meinkraft/GL/VertexData.h"

#include <vector>

class ChunkBuffer;

class ChunkBufferSegment
{
public:
	
	ChunkBufferSegment(ChunkBufferSegment&& other) = delete;
	ChunkBufferSegment& operator=(ChunkBufferSegment&& other) = delete;
	ChunkBufferSegment(const ChunkBufferSegment& other) = delete;
	ChunkBufferSegment& operator=(const ChunkBufferSegment& other) = delete;
	
	~ChunkBufferSegment();
	
	void setData(const std::vector<VertexData>& data);
	
	const ChunkBuffer& getBuffer() const;
	int getStartIndex() const;
	int getVertexCount() const;
	
private:
	ChunkBuffer& _container;
	int _startIndex = -1;
	int _vertexCount = -1;
	
	friend class ChunkBuffer;
	
	ChunkBufferSegment(ChunkBuffer& container, int startIndex, int vertexCount);
};