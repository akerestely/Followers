#pragma once

#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/GLTexture.h>
#include <Engine/Camera2D.h>

#include <Engine/Sprite.h>
#include "Engine/SpriteBatch.h"

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
	void calculateFPS();

	Engine::Window window;
	int screenWidth,screenHeight;
	GameState gameState;

	Engine::GLSLProgram colorProgram;
	Engine::Camera2D camera;

	Engine::SpriteBatch spriteBatch;

	float fps;
	float maxFps;
	float frameTime;

	float time;
};

