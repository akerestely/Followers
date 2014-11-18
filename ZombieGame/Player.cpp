#include "Player.h"
#include <sdl\SDL.h>

Player::Player(void):
	currentGunIndex(-1)
{
}

Player::~Player(void)
{
}

void Player::Init(int speed, glm::vec2 pos, Engine::InputManager *inputManager, Engine::Camera2D *camera, std::vector<Bullet> *bullets)
{
	this->speed = speed;
	this->position = pos;
	this->inputManager = inputManager;
	this->camera = camera;
	this->bullets = bullets;
	camera = camera;
	color.r = 0;
	color.b = 185;
	color.g = 0;
	health = 150;
}

void Player::AddGun(Gun *gun)
{
	guns.push_back(gun);

	// no gun equipped, equip gun.
	if(currentGunIndex == -1)
		currentGunIndex=0;
}

void Player::Update(const std::vector<std::string> &levelData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies, float deltaTime)
{
	if(inputManager->IsKeyDown(SDLK_w))
		position.y += speed * deltaTime;
	else if(inputManager->IsKeyDown(SDLK_s))
		position.y -= speed * deltaTime;
	if(inputManager->IsKeyDown(SDLK_a))
		position.x -= speed * deltaTime;
	else if(inputManager->IsKeyDown(SDLK_d))
		position.x += speed * deltaTime;

	if(inputManager->IsKeyDown(SDLK_1) && guns.size() >= 0)
		currentGunIndex = 0;
	else if(inputManager->IsKeyDown(SDLK_2) && guns.size() >= 1)
		currentGunIndex = 1;
	else if(inputManager->IsKeyDown(SDLK_3) && guns.size() >= 2)
		currentGunIndex = 2;

	if(currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = camera->ConvertScreenToWorld(inputManager->GetMouseCoords());
		glm::vec2 centerPosition = position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);
		guns[currentGunIndex]->Update(inputManager->IsKeyDown(SDL_BUTTON_LEFT), centerPosition, direction, *bullets, deltaTime);
	}

	CollideWithLevel(levelData);
}
