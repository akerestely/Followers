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

	//Getters
	int GetWidth() const { return levelData[0].size(); }
	int GetHeight() const { return levelData.size(); }
	int GetNumHumans() const { return numHumans; }
	const std::vector<std::string>& GetLevelData() const { return levelData;}
	glm::vec2 GetPlayerStartPos() const { return playerStartPos; }
	const std::vector<glm::vec2>& GetZombieStartPos() { return zombieStartPositions; }
private:
	std::vector<std::string> levelData;
	int numHumans;
	Engine::SpriteBatch spriteBatch;

	glm::vec2 playerStartPos;
	std::vector<glm::vec2> zombieStartPositions;
};

