#pragma once

#include "Meinkraft/ChunkBuffer.h"

#include <memory>
#include <vector>

class ChunkBufferSegment;

class ChunkBufferManager
{
public:
	void acquireAvailableSegment(std::unique_ptr<ChunkBufferSegment>& segment, int requestedVertexCount);
	
	const std::vector<std::unique_ptr<ChunkBuffer>>& getBuffers() const;

private:
	std::vector<std::unique_ptr<ChunkBuffer>> _buffers;
};