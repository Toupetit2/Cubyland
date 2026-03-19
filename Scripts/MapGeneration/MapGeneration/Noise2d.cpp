#include "pch.h"
#include "Noise2d.h"

float random2D(int x, int y) {
	int n = x * 374761393 + y * 668265263; // gros nombres premiers
	n = (n ^ (n >> 13)) * 1274126177;
	n = n ^ (n >> 16);

	return (n & 0x7fffffff) / float(0x7fffffff);
}

float fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Noise2d::GetNoiseValue(float x, float y)
{
	// nb case
	int i = floor(x);
	int j = floor(y);

	// position dans la case
	float u = x - i;
	float v = y - j;

	// valeur coins
	float v00 = random2D(i, j);
	float v10 = random2D(i + 1, j);
	float v01 = random2D(i, j + 1);
	float v11 = random2D(i + 1, j + 1);

	float fadeU = fade(u);
	float fadeV = fade(v);

	float a = std::lerp(v00, v10, fadeU);
	float b = std::lerp(v01, v11, fadeU);

	return std::lerp(a, b, fadeV);
}

#include <iostream>
#include <string>

std::string chars = " .:-=+*#%@";

std::string Noise2d::PrintNoise()
{
	std::string noiseString;

	int width = 80;
	int height = 40;
	float scale = 0.1f;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float n = Noise2d::GetNoiseValue(x * scale, y * scale);

			// si ton noise est [0,1]
			int index = n * (chars.size() - 1);

			noiseString += chars[index];
		}
		noiseString += "\n";
	}
	return noiseString;
}