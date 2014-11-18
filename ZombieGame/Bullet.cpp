#include "Bullet.h"
#include "Engine/ResourceMngr.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	position(position),
		direction(direction),
		damage(damage),
		speed(speed)
{

}

Bullet::~Bullet(void)
{
}

bool Bullet::Update(const std::vector<std::string> &levelData)
{
	position += direction * speed;
	return collideWithWorld(levelData);
}

void Bullet::Draw(Engine::SpriteBatch &spriteBatch)
{
	glm::vec4 destRect(position.x + BULLET_RADIUS, position.y +BULLET_RADIUS, BULLET_RADIUS*2, BULLET_RADIUS*2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.Draw(destRect, uvRect, Engine::ResourceMngr::GetTexture("Textures/circle.png").id, 0.0f, Engine::Color(75,75,75));
}

bool Bullet::CollideWithAgent(Agent *agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = position;
	glm::vec2 centerPosB = agent->GetPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;
	if(collisionDepth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string> &levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(position.y / (float)TILE_WIDTH);

	if (gridPosition.x <0 || gridPosition.x >= levelData[0].size() ||
	    gridPosition.y <0 || gridPosition.y >= levelData.size())
		return true;
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}
