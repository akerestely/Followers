#pragma once

#include <sdl/SDL.h>
#undef main

#include "GLSLProgram.h"
#include "Sprite.h"
#include "GLTexture.h"

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

	SDL_Window *window;
	int screenWidth,screenHeight;
	GameState gameState;

	Sprite sprite[3];

	GLSLProgram colorProgram;

	float fps;
	float maxFps;
	float frameTime;

	float time;
};

