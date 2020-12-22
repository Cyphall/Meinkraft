#pragma once

#include <FastNoiseLite.h>

class NoiseGenerator
{
public:
	NoiseGenerator();
	
	float getNoise(float x, float y, int octaves, float scale, float lacunarity, float persistance);
	
private:
	FastNoiseLite _noise;
	
	float getRawNoise(float x, float y, float frequency, float amplitude);
};
