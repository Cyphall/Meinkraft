#pragma once

#include <FastNoiseLite.h>

class NoiseGenerator
{
public:
	NoiseGenerator();
	
	float getNoise(float x, float y, float scale, int octaves, float lacunarity = 2.0f, float persistance = 0.5f);
	
private:
	FastNoiseLite _noise;
};
