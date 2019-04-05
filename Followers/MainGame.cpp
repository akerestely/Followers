#include "MainGame.h"

#include <Engine/Errors.h>
#include <glm/gtc/matrix_transform.hpp>

//remove
#include <iostream>
#include <Engine/ModelLoader.h>
//

MainGame::MainGame(void) :
	screenWidth(1920),
	screenHeight(1080),
	gameState(PLAY),
	maxFps(60)
{
	//empty
}

MainGame::~MainGame(void)
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

	// caution: changing to full_screen will change internal screen width and height
	window.Create("Followers", screenWidth, screenHeight, Engine::BORDERLESS);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	camera.Init(screenWidth,screenHeight);

	fpsLimiter.Init(maxFps);

	level=new Level("Resources/Map/imgn45w114_1_300");
	levelRenderer = new LevelRenderer(level);
	modelManager = new ModelManager(level);

	camera.Rotate(glm::vec3(0.0f, 90.0f, 0.0f));
	camera.Move(glm::vec3(ROW*CELL_SIZE/2, 2300.f, 0.0f));

	sky = new Engine::SkyDome;
	sun = new Engine::Sun(screenWidth, screenHeight);
	water = new Engine::Water;

	glClearColor(0.0,0.0,0.0,1.0);
	glClearDepth(1.0);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void MainGame::gameLoop()
{
	time = 0;
	while(gameState != EXIT)
	{
		fpsLimiter.Begin();

		processInput();

		update();
		time+=0.01;
		renderScene();

		fps = fpsLimiter.End();
		std::cout<<"FPS: "<<fps<<'\n';
	}
}

void MainGame::processInput()
{
	inputManager.Update();

	SDL_Event evnt;
	float CAMERA_SPEED = 1.0f;
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
		case SDL_WINDOWEVENT:
			switch(evnt.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				resize((int) evnt.window.data1, (int)evnt.window.data2);
				printf("%dx%d\n",(int) evnt.window.data1, (int)evnt.window.data2);
				break;
			}
			break;
		}
	}

	if(inputManager.IsKeyDown(SDLK_ESCAPE))
		gameState = EXIT;

	if(inputManager.IsKeyDown(SDLK_LALT))
		CAMERA_SPEED=10.0f;
	else if(inputManager.IsKeyDown(SDLK_LSHIFT))
		CAMERA_SPEED=100.0f;
	else
		CAMERA_SPEED=1.0f;

	if(inputManager.IsKeyDownOnce(SDLK_F4))
		window.Fullscreen(!window.IsFullscreen());

	if(inputManager.IsKeyDownOnce(SDLK_v))
		SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode()==SDL_TRUE ? SDL_FALSE:SDL_TRUE);

	if(inputManager.IsKeyDown(SDLK_w))
		camera.Move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_s))
		camera.Move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED));
	if(inputManager.IsKeyDown(SDLK_a))
		camera.Move(glm::vec3(-CAMERA_SPEED, 0.0f, 0.0f));
	if(inputManager.IsKeyDown(SDLK_d))
		camera.Move(glm::vec3(CAMERA_SPEED, 0.0f, 0.0f));
	if(inputManager.IsKeyDown(SDLK_SPACE))
		camera.Move(glm::vec3(0.0f, CAMERA_SPEED, 0.0f));
	if(inputManager.IsKeyDown(SDLK_c))
		camera.Move(glm::vec3(0.0f, -CAMERA_SPEED, 0.0f));

	if(inputManager.IsKeyDown(SDLK_UP))
		sun->Update(camera, 0.3f);
	if(inputManager.IsKeyDown(SDLK_DOWN))
		sun->Update(camera, -0.3f);

	if(SDL_GetRelativeMouseMode() == SDL_TRUE)
	{
		static const float MOUSE_SENSITIVITY = 0.2f;
		glm::vec2 mouseCoords = inputManager.GetMouseCoordsRel();
		camera.Rotate(glm::vec3(mouseCoords.y*MOUSE_SENSITIVITY, mouseCoords.x*MOUSE_SENSITIVITY,0.0f));
	}
	if(inputManager.IsKeyDownOnce(SDL_BUTTON_LEFT))
	{
		int x = inputManager.GetMouseCoords().x;
		int y = inputManager.GetMouseCoords().y;

		GLbyte color[4];
		GLfloat depth;
		GLuint index;

		glReadPixels(x, screenHeight - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		glReadPixels(x, screenHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glReadPixels(x, screenHeight - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
			x, y, color[0], color[1], color[2], color[3], depth, index);

		glm::vec4 viewport = glm::vec4(0, 0, screenWidth, screenHeight);
		glm::vec3 wincoord = glm::vec3(x, screenHeight - y - 1, depth);
		glm::vec3 objcoord = glm::unProject(wincoord, camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);

		printf("Coordinates in object space: %f, %f, %f\n",
			objcoord.x, objcoord.y, objcoord.z);

		modelManager->Position(objcoord.x, objcoord.y, objcoord.z);
	}
	if (inputManager.IsKeyDown(SDLK_h))
	{
		if(inputManager.IsKeyDown(SDLK_RIGHTBRACKET))
			modelManager->Height(CAMERA_SPEED);
		if(inputManager.IsKeyDown(SDLK_LEFTBRACKET))
			modelManager->Height(-CAMERA_SPEED);
	}
	if (inputManager.IsKeyDown(SDLK_x))
	{
		if(inputManager.IsKeyDown(SDLK_RIGHTBRACKET))
			modelManager->Scale(CAMERA_SPEED);
		if(inputManager.IsKeyDown(SDLK_LEFTBRACKET))
			modelManager->Scale(-CAMERA_SPEED);
	}
	if (inputManager.IsKeyDown(SDLK_r))
	{
		if(inputManager.IsKeyDown(SDLK_RIGHTBRACKET))
			modelManager->Rotate(CAMERA_SPEED);
		if(inputManager.IsKeyDown(SDLK_LEFTBRACKET))
			modelManager->Rotate(-CAMERA_SPEED);
	}
	if (inputManager.IsKeyDownOnce(SDLK_COMMA))
		modelManager->NextModel();
	if (inputManager.IsKeyDownOnce(SDLK_PERIOD))
		modelManager->NewModel();

	if (inputManager.IsKeyDown(SDLK_LCTRL))
	{
		if (inputManager.IsKeyDownOnce(SDLK_s))
			modelManager->Save();
		if (inputManager.IsKeyDownOnce(SDLK_LEFTBRACKET))
			modelManager->PreviousSelection();
	}
}

void MainGame::update()
{
	camera.Update();

	sun->Update(camera, 0.01);
}

void MainGame::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sky->Render(camera, sun->GetSunPosition());
	levelRenderer->Render(camera, sun);
	
	modelManager->Render(camera, sun);

	water->Render(camera, time);
	//Sun must be rendered last for the depth test to be good
	sun->Render(camera);

	window.SwappBuffer();
}

void MainGame::resize(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	
	camera.Init(screenWidth, screenHeight);
	sun->Resize(screenWidth, screenHeight);
}
