#pragma once

#include <array>
#include <glm/glm.hpp>
#include "Block/BlockDefinition.h"
#include "BlockId.h"

class BlockContainer
{
public:
	void setBlockId(glm::u8vec3 pos, BlockId block);
	BlockId getBlockId(glm::u8vec3 pos) const;
	
	BlockDefinition* getBlockDefinition(glm::u8vec3 pos) const;
	
	static int posToIndex(glm::u8vec3 pos);
	static glm::u8vec3 indexToPos(int index);
	
private:
	std::array<BlockId, 16 * 16 * 16> _blocks;
};
