#pragma once

#include <MathsLib/Vector2.h>
#include "Types.h"
#include "mgNoise2d.h"

class MapGeneration
{
public:
	static Types getType(Vector2<float> pos);
	
	static std::string drawMap();
private:
};