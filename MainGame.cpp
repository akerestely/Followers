#include "MainGame.h"

#include <Engine/Errors.h>

#include <Engine/ImageLoader.h>
//remove
#include <iostream>

MainGame::MainGame(void) :
	screenWidth(800),
	screenHeight(600),
	gameState(PLAY),
	time(0),
	maxFps(60)
{
	camera.Init(screenWidth,screenHeight);
}

void MainGame::Run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	Engine::Init();

	window.Create("Followers", screenWidth, screenHeight, 0);

	initShaders();

	spriteBatch.Init();
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

		camera.Update();
		renderScene();

		//calculateFPS();
		//std::cout<<fps<<"\n";

		float frameTicks = SDL_GetTicks() - startTicks;
		if(1000.0f / maxFps > frameTicks)
			SDL_Delay(1000.0f / maxFps - frameTicks);
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;
	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;
	while(SDL_PollEvent(&evnt))
	{
		switch(evnt.type)
		{
		case SDL_QUIT:
			gameState=EXIT;
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:
				camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
				break;
			case SDLK_s:
				camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
				break;
			case SDLK_a:
				camera.SetPosition(camera.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;
			case SDLK_d:
				camera.SetPosition(camera.GetPosition() + glm::vec2(CAMERA_SPEED, -0.0f));
				break;
			case SDLK_q:
				camera.SetScale(camera.GetScale() + SCALE_SPEED);
				break;	 
			case SDLK_e: 
				camera.SetScale(camera.GetScale() - SCALE_SPEED);
				break;
			}
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

	//set the camera matrix
	GLint pLocation = colorProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = camera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	spriteBatch.Begin();

	glm::vec4 pos(0,0,50,50);
	glm::vec4 uv(0,0,1,1);
	static Engine::GLTexture texture = Engine::ResourceMngr::GetTexture("Textures/PNG/HearthEnemy1.png");
	Engine::Color color;
	color.r=255;
	color.g=255;
	color.b=255;
	color.a=255;

	for(int i=0; i<100; i++)
	{
		spriteBatch.Draw(pos,uv,texture.id,0,color);
		spriteBatch.Draw(pos + glm::vec4(50,0,0,0),uv,texture.id,0,color);
	}

	spriteBatch.End();
	spriteBatch.RenderBatches();


	glBindTexture(GL_TEXTURE_2D, 0);
	colorProgram.UnUse();

	window.SwappBuffer();
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