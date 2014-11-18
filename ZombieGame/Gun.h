#pragma once

#include <string>
#include <vector>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun(void);

	void Update(bool isMouseDown, const glm::vec2& position, glm::vec2 direction, std::vector<Bullet> &bullets);

private:
	void fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet> &bullets);

	std::string name;
	int fireRate;

	int bulletsPerShot; ///< how many bullets per shot

	float spread; ///< accuracy

	float bulletSpeed;
	float bulletDamage;

	int frameCounter;
};

