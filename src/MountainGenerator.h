#pragma once

#include <FastNoiseLite.h>
#include <vector>
#include "BlockType.h"
#include "NoiseGenerator.h"

class MountainGenerator
{
public:
	std::vector<BlockType> generate(int x, int y, int z);

private:
	NoiseGenerator _noise;
	
	static constexpr int MIN_HEIGHT = 64;
	static constexpr int MAX_HEIGHT = 255;
};
