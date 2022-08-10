#pragma once

#include "Meinkraft/MountainGenerator.h"
#include "Meinkraft/BlockContainer.h"

#include <vector>

class WorldGenerator
{
public:
	BlockContainer generateMountains(int x, int y, int z);

private:
	MountainGenerator _mountains;
};
