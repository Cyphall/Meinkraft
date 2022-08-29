#include "BlockContainer.h"

#include "Meinkraft/Block/BlockManager.h"

#include <cassert>

void BlockContainer::setBlockId(glm::u8vec3 pos, BlockId block)
{
	assert(MathHelper::between(pos.x, 0, 15));
	assert(MathHelper::between(pos.y, 0, 15));
	assert(MathHelper::between(pos.z, 0, 15));
	_blocks[posToIndex(pos)] = block;
}

BlockId BlockContainer::getBlockId(glm::u8vec3 pos) const
{
	assert(MathHelper::between(pos.x, 0, 15));
	assert(MathHelper::between(pos.y, 0, 15));
	assert(MathHelper::between(pos.z, 0, 15));
	return _blocks[posToIndex(pos)];
}

int BlockContainer::posToIndex(glm::u8vec3 pos)
{
	return pos.x + pos.y * 16 + pos.z * 256;
}

glm::u8vec3 BlockContainer::indexToPos(int index)
{
	return glm::u8vec3(index % 16, (index/16) % 16, (index/256) % 16);
}

BlockDefinition& BlockContainer::getBlockDefinition(glm::u8vec3 pos) const
{
	BlockId blockId = getBlockId(pos);
	return BlockManager::getBlockDefinition(blockId);
}