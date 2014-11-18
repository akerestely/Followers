#include "Player.h"
#include <sdl\SDL_keycode.h>


Player::Player(void)
{
}


Player::~Player(void)
{
}

void Player::Init(int speed, glm::vec2 pos, Engine::InputManager *inputManager)
{
	this->speed = speed;
	this->position = pos;
	this->inputManager = inputManager; 
	color.r = 0;
	color.b = 185;
	color.g = 0;
}

void Player::Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies)
{
	if(inputManager->IsKeyPressed(SDLK_w))
		position.y += speed;
	else if(inputManager->IsKeyPressed(SDLK_s))
		position.y -= speed;
	if(inputManager->IsKeyPressed(SDLK_a))
		position.x -= speed;
	else if(inputManager->IsKeyPressed(SDLK_d))
		position.x += speed;

	CollideWithLevel(levelData);
}
