#include "ChunkBuffer.h"

ChunkBuffer::ChunkBuffer()
{
	glCreateBuffers(1, &_vbo);
	
	glNamedBufferStorage(_vbo, ChunkBuffer::BUFFER_ELEMENT_COUNT * sizeof(VertexData), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

bool ChunkBuffer::tryAcquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& newSegment, int requestedVertexCount)
{
	int startIndex = 0;
	for (const ChunkBufferSegment* segment : _bufferSegments)
	{
		if (segment->getStartIndex() - startIndex >= requestedVertexCount)
		{
			break;
		}
		startIndex = segment->getStartIndex() + segment->getVertexCount();
	}
	
	if (startIndex + requestedVertexCount < ChunkBuffer::BUFFER_ELEMENT_COUNT)
	{
		newSegment.reset(new ChunkBufferSegment(*this, startIndex, requestedVertexCount));
		_bufferSegments.insert(newSegment.get());
		return true;
	}
	
	return false;
}

void ChunkBuffer::releaseMemory(ChunkBufferSegment* segment)
{
	glInvalidateBufferSubData(_vbo, segment->getStartIndex() * sizeof(VertexData), segment->getVertexCount() * sizeof(VertexData));
	_bufferSegments.erase(segment);
}

void ChunkBuffer::setData(ChunkBufferSegment* segment, const std::vector<VertexData>& data)
{
	glNamedBufferSubData(_vbo, segment->getStartIndex() * sizeof(VertexData), data.size() * sizeof(VertexData), data.data());
}

ChunkBuffer::~ChunkBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

GLuint ChunkBuffer::getGLBuffer() const
{
	return _vbo;
}

int ChunkBuffer::getActiveSegmentCount() const
{
	return _bufferSegments.size();
}
