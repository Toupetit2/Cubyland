#include "pch.h"
#include "mapGeneration.h"

Types MapGeneration::getType(Vector2<float> pos)
{
	float fireValue = Noise2d::GetNoiseValue(pos);
	float grassValue = Noise2d::GetNoiseValue(Vector2<float>(pos.x + 10000, pos.y)); // valeur random pour avoir un autre noise
	float waterValue = Noise2d::GetNoiseValue(Vector2<float>(pos.x + 39587, pos.y));

	if (fireValue > grassValue && fireValue > waterValue)
	{
		return Fire;
	}
	else if (grassValue > waterValue)
	{
		return Grass;
	}
	return Water;
}

std::string MapGeneration::drawMap()
{
	std::string noiseString;

	int width = 80;
	int height = 40;
	float scale = 0.1f;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (getType(Vector2<float>(x/6, y/6)) == Fire)
			{
				noiseString += "F";
			}
			else if (getType(Vector2<float>(x/6, y/6)) == Water)
			{
				noiseString += "W";
			}
			else
			{
				noiseString += "G";
			}
		}
		noiseString += "\n";
	}
	return noiseString;
}