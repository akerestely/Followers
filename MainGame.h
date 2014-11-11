#pragma once

#include <sdl/SDL.h>
#undef main

#include "Sprite.h"
#include "GLSLProgram.h"

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

	SDL_Window *window;
	int screenWidth,screenHeight;
	GameState gameState;

	Sprite sprite;

	GLSLProgram colorProgram;
};

