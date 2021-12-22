#include "ChunkBufferSegment.h"
#include "ChunkBuffer.h"

ChunkBufferSegment::ChunkBufferSegment(ChunkBuffer& container, int segmentIndex):
_container(container), _segmentIndex(segmentIndex)
{

}

ChunkBufferSegment::~ChunkBufferSegment()
{
	_container.releaseMemory(_segmentIndex);
}

void ChunkBufferSegment::setData(const std::vector<VertexData>& data)
{
	assert(data.size() <= ChunkBufferSegment::CHUNK_BUFFER_SIZE);
	_container.setData(_segmentIndex, data);
}

const ChunkBuffer& ChunkBufferSegment::getBuffer() const
{
	return _container;
}

int ChunkBufferSegment::getByteOffset() const
{
	return _segmentIndex * ChunkBufferSegment::CHUNK_BUFFER_SIZE;
}

int ChunkBufferSegment::getIndexOffset() const
{
	return _segmentIndex * ChunkBufferSegment::CHUNK_BUFFER_ELEMENT_COUNT;
}
