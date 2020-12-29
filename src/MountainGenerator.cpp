#include "MountainGenerator.h"

BlockContainer MountainGenerator::generate(int x, int y, int z)
{
	BlockContainer blockContainer;
	
	for (int heightY = 0; heightY < 16; heightY++)
	{
		for (int heightX = 0; heightX < 16; heightX++)
		{
			float surfaceHeightNoise = _noise.getNoise(x + heightX / 16.0f, z + heightY / 16.0f, 6, 12, 0.4, 0.5);
			
			int surfaceHeight = MIN_HEIGHT + static_cast<int>(surfaceHeightNoise * (MAX_HEIGHT - MIN_HEIGHT));
			
			for (int height = 0; height < 16; height++)
			{
				int worldHeight = y * 16 + height;
				
				if (worldHeight < surfaceHeight - 4)
				{
					blockContainer.setBlock(heightX, height, heightY, STONE);
				}
				else if (worldHeight < surfaceHeight - 1)
				{
					blockContainer.setBlock(heightX, height, heightY, DIRT);
				}
				else if (worldHeight < surfaceHeight)
				{
					blockContainer.setBlock(heightX, height, heightY, GRASS);
				}
				else
				{
					blockContainer.setBlock(heightX, height, heightY, AIR);
				}
			}
		}
	}
	
	return blockContainer;
}
