#include "WorldGenerator.h"

BlockContainer WorldGenerator::generateMountains(int x, int y, int z)
{
	return _mountains.generate(x, y, z);
}
