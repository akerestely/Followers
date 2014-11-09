#pragma once

#include <sdl/SDL.h>
#undef main

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
	void renderScene();

	SDL_Window *window;
	int screenWidth,screenHeight;
	GameState gameState;
};

