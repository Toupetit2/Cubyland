#pragma once
#include <vector>

#include <MathsLib/Vector2.h>

class Noise2d {
public:
	static float GetNoiseValue(Vector2<float> pos);
	static std::string PrintNoise();
	
private:
	static float fade(float t);
	static float random2D(Vector2<int> pos);
};