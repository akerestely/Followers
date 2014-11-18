#include "Zombie.h"

#include "Human.h"

Zombie::Zombie(void)
{
}


Zombie::~Zombie(void)
{
}

void Zombie::Init(float speed, glm::vec2 position)
{
	this->speed = speed;
	this->position = position;
	health = 150;
	color.r=0;
	color.g=160;
	color.b=0;
}

void Zombie::Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies, float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);
	if(closestHuman)
	{
		glm::vec2 dir = glm::normalize((closestHuman->GetPosition() - position));
		position += dir * speed * deltaTime;
	}
	CollideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*> &humans)
{
	Human *closestHuman = nullptr;
	float smallestDistance = 99999999.0f;
	for(int i=0; i<humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->GetPosition() - position;
		float distance = glm::length(distVec);

		if(distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
