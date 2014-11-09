#include "MainGame.h"
#include <iostream>
#include <GL\glew.h>

void fatalError(char* msg)
{
	std::cout<<msg<<'\n';
	system("pause");
	SDL_Quit();
}

MainGame::MainGame(void)
{
	window = nullptr;
	screenWidth=800;
	screenHeight=640;
	gameState=PLAY;
}

void MainGame::Run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Followers",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screenWidth,screenHeight,SDL_WINDOW_OPENGL);

	if(window==nullptr)
		fatalError("SDL window could not be loaded");

	SDL_GLContext glContex = SDL_GL_CreateContext(window);
	if(glContex == nullptr)
		fatalError("SQL_GL context could not be created");

	GLenum glewError=glewInit();
	if(glewError != GLEW_OK)
		fatalError("Could not initialize glew!");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0,0.0,0.0,1);
}
void MainGame::gameLoop()
{
	while(gameState != EXIT)
	{
		processInput();
		renderScene();
	}
}
void MainGame::processInput()
{
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt))
	{
		switch(evnt.type)
		{
		case SDL_QUIT:
			gameState=EXIT;
			break;
		}
	}
}
void MainGame::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw here

	SDL_GL_SwapWindow(window);
}

MainGame::~MainGame(void)
{
}
