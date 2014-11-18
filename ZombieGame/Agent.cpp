#include "Agent.h"
#include <Engine\ResourceMngr.h>

#include "Level.h"

Agent::Agent(void)
{
}


Agent::~Agent(void)
{
}

bool Agent::CollideWithLevel(const std::vector<std::string> &levelData)
{
	std::vector<glm::vec2> collideTilePos;
	
	//Check four corners
	checkTilePos(levelData, collideTilePos, position.x, position.y);
	checkTilePos(levelData, collideTilePos, position.x + AGENT_WIDTH, position.y);
	checkTilePos(levelData, collideTilePos, position.x, position.y + AGENT_WIDTH);
	checkTilePos(levelData, collideTilePos, position.x + AGENT_WIDTH, position.y + AGENT_WIDTH);

	if(collideTilePos.size()==0)
		return false;

	for (int i=0; i<collideTilePos.size(); i++)
		collideWithTile(collideTilePos[i]);

	return true;
}

bool Agent::CollideWithAgent(Agent *agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;
	glm::vec2 centerPosA = position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->position + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;
	if(collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		position += collisionDepthVec / 2.0f;
		agent->position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

void Agent::Draw(Engine::SpriteBatch &spriteBatch)
{
	static int texture = Engine::ResourceMngr::GetTexture("Textures/circle.png").id;
	glm::vec4 uvRect(0.f,0.f,1.f,1.f);
	glm::vec4 destRect(position, AGENT_WIDTH, AGENT_WIDTH);

	spriteBatch.Draw(destRect, uvRect, texture, 0.0f, color);
}

bool Agent::ApplyDamage(float damage)
{
	health -= damage;
	return (health <= 0);

}

void Agent::checkTilePos(const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePos, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / TILE_WIDTH), floor(y / TILE_WIDTH) );

	//if(cornerPos.x < 0 || cornerPos.x >= levelData[0].size())
	if(levelData[cornerPos.y][cornerPos.x] != '.')
		collideTilePos.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2(TILE_WIDTH / 2.0f));
}

//AABB collision
void Agent::collideWithTile(glm::vec2 tilePos)
{
	const float TILE_RADIUS = TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if(xDepth > 0 || yDepth > 0)
	{
		if(std::max(xDepth, 0.0f) < std::max(yDepth,0.0f))
		{
			if(distVec.x < 0)
				position.x -= xDepth;
			else
				position.x += xDepth;
		}
		else
		{
			if (distVec.y < 0)
				position.y -= yDepth;
			else
				position.y += yDepth;
		}
	}
}
