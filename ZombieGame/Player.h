#pragma once
#include "Human.h"
#include <Engine/InputManager.h>
#include "Engine/Camera2D.h"

#include "Gun.h"

class Player : public Human
{
public:
	Player(void);
	~Player(void);

	void Init(int speed, glm::vec2 pos, Engine::InputManager *inputManager, Engine::Camera2D *camera, std::vector<Bullet> *bullets);

	void AddGun(Gun *gun);

	void Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies, float deltaTime) override;
private:
	Engine::InputManager *inputManager;
	std::vector<Gun*> guns;
	int currentGunIndex;

	Engine::Camera2D *camera;

	std::vector<Bullet> *bullets;
};

