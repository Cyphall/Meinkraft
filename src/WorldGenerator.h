#pragma once

#include <vector>
#include "BlockType.h"
#include "MountainGenerator.h"

class WorldGenerator
{
public:
	std::vector<BlockType> generateMountains(int x, int y, int z);

private:
	MountainGenerator _mountains;
};
