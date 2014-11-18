#pragma once

#include <glm/glm.hpp>

#include <Engine/SpriteBatch.h>

const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
	Agent(void);
	virtual ~Agent(void);

	virtual void Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies) = 0;

	bool CollideWithLevel(const std::vector<std::string> &levelData);

	bool CollideWithAgent(Agent *agent);

	void Draw(Engine::SpriteBatch &spriteBatch);

	glm::vec2 GetPosition() const { return position; }
protected:
	void checkTilePos(const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePos, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 position;
	Engine::Color color;
	float speed;
};

