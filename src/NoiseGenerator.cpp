#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator()
{
	_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	_noise.SetFrequency(1);
}

float NoiseGenerator::getNoise(float x, float y, int octaves, float scale, float lacunarity, float persistance)
{
	x /= scale;
	y /= scale;
	
	float result = 0;
	
	float divider = 0;
	
	float frequency = 1;
	float amplitude = 1;
	
	for (int i = 0; i < octaves; i++)
	{
		result += getRawNoise(x, y, frequency, amplitude);
		
		divider += amplitude;
		
		frequency *= lacunarity;
		amplitude *= persistance;
	}
	
	result /= divider;
	
	return result * 0.5f + 0.5f;
}

float NoiseGenerator::getRawNoise(float x, float y, float frequency, float amplitude)
{
	x *= frequency;
	y *= frequency;
	
	return _noise.GetNoise(x, y) * amplitude;
}
