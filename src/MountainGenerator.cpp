#include "MountainGenerator.h"

BlockContainer MountainGenerator::generate(int chunkX, int chunkY, int chunkZ)
{
	BlockContainer blockContainer;
	
	for (int blockZ = 0; blockZ < 16; blockZ++)
	{
		for (int blockX = 0; blockX < 16; blockX++)
		{
			float surfaceHeightNoise = _noise.getNoise(chunkX + blockX / 16.0f, chunkZ + blockZ / 16.0f, 6, 12, 0.4, 0.5);
			
			int surfaceHeight = MIN_HEIGHT + static_cast<int>(surfaceHeightNoise * (MAX_HEIGHT - MIN_HEIGHT));
			
			for (int blockY = 0; blockY < 16; blockY++)
			{
				int worldSpaceBlockY = chunkY * 16 + blockY;
				
				if (worldSpaceBlockY < surfaceHeight - 4)
				{
					blockContainer.setBlock(blockX, blockY, blockZ, STONE);
				}
				else if (worldSpaceBlockY < surfaceHeight - 1)
				{
					blockContainer.setBlock(blockX, blockY, blockZ, DIRT);
				}
				else if (worldSpaceBlockY < surfaceHeight)
				{
					blockContainer.setBlock(blockX, blockY, blockZ, GRASS);
				}
				else
				{
					blockContainer.setBlock(blockX, blockY, blockZ, AIR);
				}
			}
		}
	}
	
	return blockContainer;
}
