#pragma once

#include "Meinkraft/NoiseGenerator.h"
#include "Meinkraft/BlockContainer.h"

#include <FastNoiseLite.h>
#include <vector>

class MountainGenerator
{
public:
	BlockContainer generate(int x, int y, int z);

private:
	NoiseGenerator _noise;
	
	static constexpr int MIN_HEIGHT = 64;
	static constexpr int MAX_HEIGHT = 255;
};
