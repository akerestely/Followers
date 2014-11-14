#include "MainGame.h"

#include <GL\glew.h>

#include "Errors.h"

#include "ImageLoader.h"
//remove
#include <iostream>

MainGame::MainGame(void) :
	window(nullptr),
	screenWidth(800),
	screenHeight(600),
	gameState(PLAY),
	time(0),
	maxFps(60)
{
}

void MainGame::Run()
{
	initSystems();

	sprite[0].Init(-1.0f,-0.5f,1.0f,1.0f,"Textures/PNG/HearthEnemy1.png");
	sprite[1].Init(-0.0f,-0.5f,1.0f,1.0f,"Textures/PNG/HearthEnemy1.png");
	sprite[2].Init(-0.5f,-0.5f,1.0f,1.0f,"Textures/PNG/HearthEnemy1.png");

	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Followers",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screenWidth,screenHeight,SDL_WINDOW_OPENGL);

	if(window==nullptr)
		fatalError("SDL window could not be loaded");

	SDL_GLContext glContex = SDL_GL_CreateContext(window);
	if(glContex == nullptr)
		fatalError("SQL_GL context could not be created");

	GLenum glewError=glewInit();
	if(glewError != GLEW_OK)
		fatalError("Could not initialize glew!");

	printf("***   OpenGl Version: %s   ***\n", glGetString(GL_VERSION));

	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	//set v-sync
	//SDL_GL_SetSwapInterval(1);

	initShaders();
}
void MainGame::initShaders()
{
	colorProgram.CompileShaders("shaders/colorShading.vert","shaders/colorShading.frag");
	colorProgram.AddAttribute("vertexPosition");
	colorProgram.AddAttribute("vertexColor");
	colorProgram.AddAttribute("vertexUV");
	colorProgram.LinkShader();
}

void MainGame::gameLoop()
{
	while(gameState != EXIT)
	{
		float startTicks = SDL_GetTicks();

		processInput();
		time += 0.01;
		renderScene();
		calculateFPS();
		std::cout<<fps<<"\n";

		float frameTicks = SDL_GetTicks() - startTicks;
		if(1000.0f / maxFps > frameTicks)
			SDL_Delay(1000.0f / maxFps - frameTicks);
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
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = colorProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = colorProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, time);

	for(int i=0;i<3;i++)
		sprite[i].Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	colorProgram.UnUse();

	SDL_GL_SwapWindow(window);
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks;
	currentTicks = SDL_GetTicks();

	frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

	prevTicks = currentTicks;

	int count;
	currentFrame++;
	if(currentFrame < NUM_SAMPLES)
		count = currentFrame;
	else
		count = NUM_SAMPLES;

	float frameTimeAverage = 0;
	for(int i=0; i<count; i++)
		frameTimeAverage += frameTimes[i];
	frameTimeAverage /= count;

	if(frameTimeAverage>0)
		fps = 1000.0f / frameTimeAverage;
	else
		fps = 0;
}

MainGame::~MainGame(void)
{
}