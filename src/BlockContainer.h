#pragma once

#include "BlockType.h"
#include <array>
#include <glm/glm.hpp>

class BlockContainer
{
public:
	void setBlock(glm::u8vec3 pos, BlockType block);
	BlockType getBlock(glm::u8vec3 pos) const;
	
	static int posToIndex(glm::u8vec3 pos);
	static glm::u8vec3 indexToPos(int index);
	
private:
	std::array<BlockType, 16*16*16> _blocks;
};
