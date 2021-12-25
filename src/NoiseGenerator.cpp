#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator()
{
	_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	_noise.SetFrequency(1);
}

float NoiseGenerator::getNoise(float x, float y, float scale, int octaves, float lacunarity, float persistance)
{
	x /= scale;
	y /= scale;
	
	float result = 0;
	
	float divider = 0;
	
	float frequency = 1;
	float amplitude = 1;
	
	for (int i = 0; i < octaves; i++)
	{
		x *= frequency;
		y *= frequency;
		
		result += _noise.GetNoise(x, y) * amplitude;
		
		divider += amplitude;
		
		frequency *= lacunarity;
		amplitude *= persistance;
	}
	
	result /= divider;
	
	return result * 0.5f + 0.5f;
}
