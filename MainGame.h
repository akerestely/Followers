#pragma once

#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/CameraSpectator.h>
#include "Engine/InputManager.h"
#include "Engine/Timing.h"

#include "Level.h"

//remove
#include "Engine\Model.h"
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
	void initShaders();
	void gameLoop();
	void processInput();
	void renderScene();

	Engine::Window window;
	int screenWidth,screenHeight;
	GameState gameState;

	Engine::GLSLProgram colorProgram;
	Engine::CameraSpectator camera;

	Engine::InputManager inputManager;
	Engine::FpsLimiter fpsLimiter;

	float maxFps;
	float fps;

	Level *l;
	Model *m;

	//remove
	glm::vec3 movement;
	//
};

