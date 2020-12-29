#pragma once

#include <vector>
#include "BlockType.h"
#include "MountainGenerator.h"
#include "BlockContainer.h"

class WorldGenerator
{
public:
	BlockContainer generateMountains(int x, int y, int z);

private:
	MountainGenerator _mountains;
};
