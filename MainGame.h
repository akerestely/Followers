#pragma once

#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/CameraSpectator.h>
#include "Engine/InputManager.h"
#include "Engine/Timing.h"

#include "Level.h"

//remove
#include "Engine/Model.h"
#include "Engine/Mesh.h"
#include "Engine/SkyDome.h"
#include "Engine/Sun.h"
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

	Engine::GLSLProgram terrainProgram;
	Engine::GLSLProgram modelProgram;

	Engine::CameraSpectator camera;

	Engine::InputManager inputManager;
	Engine::FpsLimiter fpsLimiter;

	float maxFps;
	float fps;

	Level *l;
	Engine::Model *m;
	Engine::SkyDome *sky;
	Engine::Sun *sun;

	//remove
	Engine::Mesh mesh;
	glm::vec3 movement;
	glm::vec3 lightPos;
	//
};

