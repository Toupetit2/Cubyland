#pragma once
#include <vector>

#include <MathsLib/Vector2.h>

class Noise2d {
public:
	Noise2d(int width, int height, float scale);

	static float GetNoiseValue(float x, float y);
	static std::string PrintNoise();
	
};