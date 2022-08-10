#include "ChunkBufferSegment.h"

#include "Meinkraft/ChunkBuffer.h"

ChunkBufferSegment::ChunkBufferSegment(ChunkBuffer& container, int startIndex, int vertexCount):
_container(container), _startIndex(startIndex), _vertexCount(vertexCount)
{

}

ChunkBufferSegment::~ChunkBufferSegment()
{
	_container.releaseMemory(this);
}

void ChunkBufferSegment::setData(const std::vector<VertexData>& data)
{
	assert(data.size() == _vertexCount);
	_container.setData(this, data);
}

const ChunkBuffer& ChunkBufferSegment::getBuffer() const
{
	return _container;
}

int ChunkBufferSegment::getStartIndex() const
{
	return _startIndex;
}

int ChunkBufferSegment::getVertexCount() const
{
	return _vertexCount;
}