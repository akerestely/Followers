#pragma once
#include "Human.h"
#include <Engine\InputManager.h>

class Player : public Human
{
public:
	Player(void);
	~Player(void);

	void Init(int speed, glm::vec2 pos, Engine::InputManager *inputManager);

	void Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies);
private:
	Engine::InputManager *inputManager;
};

