#include <cassert>
#include "BlockContainer.h"

void BlockContainer::setBlock(int x, int y, int z, BlockType block)
{
	assert(x >= 0 && x <= 15);
	assert(y >= 0 && y <= 15);
	assert(z >= 0 && z <= 15);
	_blocks[x + y * 16 + z * 256] = block;
}

BlockType BlockContainer::getBlock(int x, int y, int z) const
{
	assert(x >= 0 && x <= 15);
	assert(y >= 0 && y <= 15);
	assert(z >= 0 && z <= 15);
	return _blocks[x + y * 16 + z * 256];
}
