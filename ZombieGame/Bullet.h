#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Engine/SpriteBatch.h"
#include "Agent.h"

class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet(void);

	bool Update(const std::vector<std::string> &levelData);

	void Draw(Engine::SpriteBatch &spriteBatch);

	bool CollideWithAgent(Agent *agent);

	//getters
	float GetDamage() const { return damage; }
private:
	bool collideWithWorld(const std::vector<std::string> &levelData);

	float damage;
	glm::vec2 position;
	glm::vec2 direction;
	float speed;
};

