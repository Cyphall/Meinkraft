#pragma once

#include "ChunkBufferSegment.h"
#include <set>
#include <glad/gl.h>
#include <memory>

class ChunkBuffer
{
public:
	static inline constexpr int BUFFER_ELEMENT_COUNT = 1000000;
	
	~ChunkBuffer();
	ChunkBuffer(ChunkBuffer&& other) = delete;
	ChunkBuffer& operator=(ChunkBuffer&& other) = delete;
	ChunkBuffer(const ChunkBuffer& other) = delete;
	ChunkBuffer& operator=(const ChunkBuffer& other) = delete;
	
	GLuint getGLBuffer() const;
	
	int getActiveSegmentCount() const;

private:
	struct BufferSegmentCompare
	{
		bool operator()(ChunkBufferSegment* a, ChunkBufferSegment* b) const
		{
			return a->getStartIndex() < b->getStartIndex();
		}
	};
	
	std::set<ChunkBufferSegment*, BufferSegmentCompare> _bufferSegments;
	GLuint _vbo;
	
	friend class ChunkBufferSegment;
	friend class ChunkBufferManager;
	
	ChunkBuffer();
	bool tryAcquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& newSegment, int requestedVertexCount);
	void releaseMemory(ChunkBufferSegment* segment);
	void setData(ChunkBufferSegment* segment, const std::vector<VertexData>& data);
};
