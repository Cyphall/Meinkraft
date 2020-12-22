#include "MountainGenerator.h"

std::vector<BlockType> MountainGenerator::generate(int x, int y, int z)
{
	std::vector<BlockType> blocks;
	blocks.resize(16*16*16);
	
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
					blocks[heightX + height*16 + heightY*256] = STONE;
				}
				else if (worldHeight < surfaceHeight - 1)
				{
					blocks[heightX + height*16 + heightY*256] = DIRT;
				}
				else if (worldHeight < surfaceHeight)
				{
					blocks[heightX + height*16 + heightY*256] = GRASS;
				}
				else
				{
					blocks[heightX + height*16 + heightY*256] = AIR;
				}
			}
		}
	}
	
	return blocks;
}
