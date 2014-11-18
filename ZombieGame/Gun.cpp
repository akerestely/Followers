#include "Gun.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	name(name), 
	fireRate(fireRate), 
	bulletsPerShot(bulletsPerShot), 
	spread(spread), 
	bulletDamage(bulletDamage), 
	bulletSpeed(bulletSpeed),
	frameCounter(0)
{

}

Gun::~Gun(void)
{
}

void Gun::Update(bool isMouseDown,const glm::vec2& position, glm::vec2 direction, std::vector<Bullet> &bullets, float deltaTime)
{
	frameCounter += 1.0f * deltaTime;
	if(frameCounter >= fireRate && isMouseDown)
	{
		fire(direction, position, bullets);
		frameCounter = 0;
	}
}

void Gun::fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet> &bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRot(-spread, spread);

	for (int i=0; i<bulletsPerShot; i++)
	{
		bullets.push_back(Bullet(position, glm::rotate(direction, randRot(randomEngine)), bulletDamage, bulletSpeed));
	}
}
