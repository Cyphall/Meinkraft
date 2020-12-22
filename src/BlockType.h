#pragma once

#include <stdint.h>

enum BlockType : uint8_t
{
	STONE = 0,
	GRASS = 1,
	DIRT = 2,
	WOOD = 3,
	IRON = 4,
	
	AIR = 255
};