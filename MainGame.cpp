#include "MainGame.h"

#include <Engine/Errors.h>

#include <Engine/ImageLoader.h>
//remove
#include <iostream>

MainGame::MainGame(void) :
	screenWidth(800),
	screenHeight(600),
	gameState(PLAY),
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

	fpsLimiter.Init(maxFps);
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
		fpsLimiter.Begin();

		processInput();

		camera.Update();
		renderScene();

		fps = fpsLimiter.End();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;
	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;
	while(SDL_PollEvent(&evnt))
	{
		switch(evnt.type)
		{
		case SDL_QUIT:
			gameState=EXIT;
			break;
		case SDL_KEYDOWN:
			inputManager.PressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.ReleaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.PressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.ReleaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager.SetMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		}
	}

	if(inputManager.IsKeyDown(SDLK_w))
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_s))
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_a))
		camera.SetPosition(camera.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	if(inputManager.IsKeyDown(SDLK_d))
		camera.SetPosition(camera.GetPosition() + glm::vec2(CAMERA_SPEED, -0.0f));
	if(inputManager.IsKeyDown(SDLK_q))
		camera.SetScale(camera.GetScale() + SCALE_SPEED);
	if(inputManager.IsKeyDown(SDLK_e))
		camera.SetScale(camera.GetScale() - SCALE_SPEED);

	if(inputManager.IsKeyDown(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = inputManager.GetMouseCoords();
		mouseCoords = camera.ConvertScreenToWorld(mouseCoords);
	}
}

void MainGame::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	colorProgram.Use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = colorProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//set the camera matrix
	GLint pLocation = colorProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = camera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	//actual drawing here


	glBindTexture(GL_TEXTURE_2D, 0);
	colorProgram.UnUse();

	window.SwappBuffer();
}

MainGame::~MainGame(void)
{
}