#pragma once

#include "ChunkBufferSegment.h"
#include <array>
#include <glad/gl.h>
#include <memory>

class ChunkBuffer
{
public:
	static inline constexpr int CHUNK_COUNT = 100;
	
	~ChunkBuffer();
	ChunkBuffer(ChunkBuffer&& other) = delete;
	ChunkBuffer& operator=(ChunkBuffer&& other) = delete;
	ChunkBuffer(const ChunkBuffer& other) = delete;
	ChunkBuffer& operator=(const ChunkBuffer& other) = delete;
	
	GLuint getGLBuffer() const;

private:
	std::array<ChunkBufferSegment*, CHUNK_COUNT> _bufferSegments = {};
	GLuint _vbo;
	
	friend class ChunkBufferSegment;
	friend class ChunkBufferManager;
	
	ChunkBuffer();
	void releaseMemory(int index);
	void setData(int index, const std::vector<VertexData>& data);
	bool tryAcquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& segment);
};
