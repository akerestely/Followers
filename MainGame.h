#pragma once

#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/CameraSpectator.h>
#include "Engine/InputManager.h"
#include "Engine/Timing.h"

#include "Engine/SkyDome.h"
#include "Engine/Sun.h"
#include "Engine/Water.h"

#include "Level.h"
#include "LevelRenderer.h"

//remove
#include "Engine/Model.h"
//

enum GameState {PLAY,EXIT}; 

class MainGame
{
public:
	MainGame(void);
	void Run();
	~MainGame(void);
private:
	void initSystems();
	void gameLoop();
	void processInput();
	void update();
	void renderScene();
	void resize(int screenWidth, int screenHeight);

	Engine::Window window;
	int screenWidth,screenHeight;
	GameState gameState;

	Engine::CameraSpectator camera;

	Engine::InputManager inputManager;
	Engine::FpsLimiter fpsLimiter;

	float maxFps;
	float fps;

	Level *l;
	LevelRenderer *levelRenderer;
	Engine::SkyDome *sky;
	Engine::Sun *sun;
	Engine::Water *water;
	Engine::Model *m;

	//remove
	glm::vec3 movement;
	float time;
	//
};

