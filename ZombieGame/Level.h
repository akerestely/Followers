#pragma once

#include <string>
#include <vector>

#include "Engine/SpriteBatch.h"

const int TILE_WIDTH = 64;

class Level
{
public:
	//Load the level
	Level(const std::string &fileName);
	~Level(void);

	void Draw();
private:
	std::vector<std::string> levelData;
	int numHumans;
	Engine::SpriteBatch spriteBatch;

	glm::ivec2 playerStartPos;
	std::vector<glm::ivec2> zombieStartPositions;
};

