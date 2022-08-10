#pragma once

#include <cstdint>

enum class BlockId : uint8_t
{
	STONE = 0,
	GRASS = 1,
	DIRT = 2,
	WOOD = 3,
	IRON = 4,
	
	AIR = 255
};