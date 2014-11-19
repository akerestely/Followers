#include "Human.h"
#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>

Human::Human(void)
{
}


Human::~Human(void)
{
}

void Human::Init(float speed, glm::vec2 position)
{
	this->speed = speed;
	this->position = position;
	color.r=200;
	color.g=0;
	color.b=200;
	health = 20;

	do
	{
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

		direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	}while(direction.length() == 0);

	direction = glm::normalize(direction);
	
}

void Human::Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies, float deltaTime)
{
	position += direction * speed * deltaTime;

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	static int frame = 0;
	if (frame == 20)
	{
		direction = glm::rotate(direction, randRot(randomEngine));
		frame=0;
	}
	else
		frame++;

	if(CollideWithLevel(levelData))
		direction = glm::rotate(direction, randRot(randomEngine));
}