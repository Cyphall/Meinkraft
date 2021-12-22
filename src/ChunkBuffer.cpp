#include "ChunkBuffer.h"

ChunkBuffer::ChunkBuffer()
{
	glCreateBuffers(1, &_vbo);
	
	glNamedBufferStorage(_vbo, ChunkBuffer::CHUNK_COUNT * ChunkBufferSegment::CHUNK_BUFFER_SIZE, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

bool ChunkBuffer::tryAcquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& segment)
{
	for (int i = 0; i < _bufferSegments.size(); i++)
	{
		if (!_bufferSegments[i])
		{
			segment.reset(new ChunkBufferSegment(*this, i));
			_bufferSegments[i] = segment.get();
			return true;
		}
	}
	
	return false;
}

void ChunkBuffer::releaseMemory(int index)
{
	glInvalidateBufferSubData(_vbo, index * ChunkBufferSegment::CHUNK_BUFFER_SIZE, ChunkBufferSegment::CHUNK_BUFFER_SIZE);
	_bufferSegments[index] = nullptr;
}

void ChunkBuffer::setData(int index, const std::vector<VertexData>& data)
{
	glNamedBufferSubData(_vbo, index * ChunkBufferSegment::CHUNK_BUFFER_SIZE, data.size() * sizeof(VertexData), data.data());
}

ChunkBuffer::~ChunkBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

GLuint ChunkBuffer::getGLBuffer() const
{
	return _vbo;
}
