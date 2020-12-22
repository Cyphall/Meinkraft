#include "WorldGenerator.h"

std::vector<BlockType> WorldGenerator::generateMountains(int x, int y, int z)
{
	return _mountains.generate(x, y, z);
}
