#pragma once

#include "BlockType.h"
#include <array>

class BlockContainer
{
public:
	void setBlock(int x, int y, int z, BlockType block);
	BlockType getBlock(int x, int y, int z) const;
	
private:
	std::array<BlockType, 16*16*16> _blocks;
};
