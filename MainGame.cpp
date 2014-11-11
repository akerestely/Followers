#include "MainGame.h"

#include <GL\glew.h>

#include "Errors.h"

MainGame::MainGame(void) :
	window(nullptr),
	screenWidth(800),
	screenHeight(600),
	gameState(PLAY),
	time(0)
{
}

void MainGame::Run()
{
	initSystems();

	sprite.Init(-0.5f,-0.5f,1.0f,1.0f);

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

	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	initShaders();
}
void MainGame::initShaders()
{
	colorProgram.CompileShaders("shaders/colorShading.vert","shaders/colorShading.frag");
	colorProgram.AddAttribute("vertexPosition");
	colorProgram.AddAttribute("vertexColor");
	colorProgram.LinkShader();
}

void MainGame::gameLoop()
{
	while(gameState != EXIT)
	{
		processInput();
		time += 0.01;
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	colorProgram.Use();

	GLuint timeLocation = colorProgram.GetUniformLocation("time");
	glUniform1f(timeLocation,time);

	sprite.Draw();

	colorProgram.UnUse();

	SDL_GL_SwapWindow(window);
}

MainGame::~MainGame(void)
{
}
