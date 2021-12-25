#include "MountainGenerator.h"

BlockContainer MountainGenerator::generate(int chunkX, int chunkY, int chunkZ)
{
	BlockContainer blockContainer;
	
	glm::u8vec3 blockPos;
	for (blockPos.z = 0; blockPos.z < 16; blockPos.z++)
	{
		for (blockPos.x = 0; blockPos.x < 16; blockPos.x++)
		{
			float flatness = _noise.getNoise(chunkX + blockPos.x / 16.0f, chunkZ + blockPos.z / 16.0f, 20, 1);
			float surfaceHeightNoise = _noise.getNoise(chunkX + blockPos.x / 16.0f, chunkZ + blockPos.z / 16.0f, 12, 3, 2, (1 - flatness) * 0.4f);
			surfaceHeightNoise *= surfaceHeightNoise;
			surfaceHeightNoise = glm::smoothstep(0.0f, 0.8f, surfaceHeightNoise);
			
			int surfaceHeight = std::lerp(MIN_HEIGHT, MAX_HEIGHT, surfaceHeightNoise);
			
			for (blockPos.y = 0; blockPos.y < 16; blockPos.y++)
			{
				int worldSpaceBlockY = chunkY * 16 + blockPos.y;
				
				if (worldSpaceBlockY < surfaceHeight - 4)
				{
					blockContainer.setBlock(blockPos, STONE);
				}
				else if (worldSpaceBlockY < surfaceHeight - 1)
				{
					blockContainer.setBlock(blockPos, DIRT);
				}
				else if (worldSpaceBlockY < surfaceHeight)
				{
					blockContainer.setBlock(blockPos, GRASS);
				}
				else
				{
					blockContainer.setBlock(blockPos, AIR);
				}
			}
		}
	}
	
	return blockContainer;
}
