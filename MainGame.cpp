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
	//empty
}

void MainGame::Run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	Engine::Init();

	window.Create("Followers", screenWidth, screenHeight, /*Engine::FULLSCREEN*/0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	initShaders();

	camera.Init(screenWidth,screenHeight);

	fpsLimiter.Init(maxFps);

	l=new Level("Resources/Map/imgn45w114_1");
	glClearColor(0.5,0.5,0.5,0.5);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);
}

void MainGame::initShaders()
{
	colorProgram.CompileShaders("shaders/colorShading.vert","shaders/colorShading.frag");
	colorProgram.AddAttribute("vertexPosition");
	colorProgram.AddAttribute("vertexColor");
	//colorProgram.AddAttribute("vertexUV");
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
		std::cout<<fps<<'\n';
	}
}

void MainGame::processInput()
{
	inputManager.Update();

	SDL_Event evnt;
	float CAMERA_SPEED = 1.1f;
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
			if(SDL_GetRelativeMouseMode() == SDL_TRUE)
				inputManager.SetMouseCoordsRel(evnt.motion.xrel, evnt.motion.yrel);
			else
				inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		}
	}

	if(inputManager.IsKeyDown(SDLK_ESCAPE))
		gameState = EXIT;
// 		if(SDL_GetRelativeMouseMode() == SDL_TRUE)
// 			SDL_SetRelativeMouseMode(SDL_FALSE);
// 		else
// 			SDL_SetRelativeMouseMode(SDL_TRUE);

	if(inputManager.IsKeyDown(SDLK_LSHIFT))
		CAMERA_SPEED=10;
	else
		CAMERA_SPEED=1.1;

	if(inputManager.IsKeyDownOnce(SDLK_F1))
		l->SwitchWireframeVisibility();
	if(inputManager.IsKeyDownOnce(SDLK_F4))
		window.Fullscreen(!window.IsFullscreen());

	if(inputManager.IsKeyDown(SDLK_w))
		camera.Move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_s))
		camera.Move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_a))
		camera.Move(glm::vec3(-CAMERA_SPEED, 0.0f, 0.0f));
	if(inputManager.IsKeyDown(SDLK_d))
		camera.Move(glm::vec3(CAMERA_SPEED, 0.0f, 0.0f));
	if(inputManager.IsKeyDown(SDLK_SPACE))
		if(!inputManager.IsKeyDown(SDLK_LCTRL))
			camera.Move(glm::vec3(0.0f, CAMERA_SPEED, 0.0f));
		else
			camera.Move(glm::vec3(0.0f, -CAMERA_SPEED, 0.0f));

	if(SDL_GetRelativeMouseMode() == SDL_TRUE)
	{
		static const float MOUSE_SENSITIVITY = 0.2f;
		glm::vec2 mouseCoords = inputManager.GetMouseCoordsRel();
		camera.Rotate(glm::vec3(mouseCoords.y*MOUSE_SENSITIVITY, mouseCoords.x*MOUSE_SENSITIVITY,0.0f));
	}
}

void MainGame::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	colorProgram.Use();
	//glActiveTexture(GL_TEXTURE0);

	//GLint textureLocation = colorProgram.GetUniformLocation("mySampler");
	//glUniform1i(textureLocation, 0);

	//set the camera matrix
	GLint pLocation = colorProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = camera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	//actual drawing here
	l->Render();

	//glBindTexture(GL_TEXTURE_2D, 0);
	colorProgram.UnUse();

	window.SwappBuffer();
}

MainGame::~MainGame(void)
{
}