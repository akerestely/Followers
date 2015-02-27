#include "MainGame.h"

#include <Engine/Errors.h>

#include <Engine/ImageLoader.h>
//remove
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <Engine/ModelLoader.h>
//

MainGame::MainGame(void) :
	screenWidth(1024),
	screenHeight(768),
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
	camera.Rotate(glm::vec3(0.0f, 180.0f, 0.0f));
	camera.Move(glm::vec3(0.0f, 2300.f, 0.0f));

	fpsLimiter.Init(maxFps);

	l=new Level("Resources/Map/imgn45w114_1");

	//remove
	terrainProgram.Use();
	GLint textureLocation = terrainProgram.GetUniformLocation("mySampler0");
	glUniform1i(textureLocation, 0);
	textureLocation = terrainProgram.GetUniformLocation("mySampler1");
	glUniform1i(textureLocation, 1);
	textureLocation = terrainProgram.GetUniformLocation("mySampler2");
	glUniform1i(textureLocation, 2);

	float maxHeight, minHeight;
	l->GetMaxMinHeight(maxHeight, minHeight);
	GLint heightLocation = terrainProgram.GetUniformLocation("maxHeight");
	glUniform1f(heightLocation, maxHeight);
	heightLocation = terrainProgram.GetUniformLocation("minHeight");
	glUniform1f(heightLocation, minHeight);
	terrainProgram.UnUse();

	m = Engine::ModelLoader::LoadAssimp("Resources/Models/Grass/grass_01.obj");
	//m = Engine::ModelLoader::LoadAssimp("Resources/Models/Boy/boy.3ds");
	lightPos = glm::vec3(0,2000,0);
	//
	glClearColor(0.5,0.5,0.5,0.5);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);
}

void MainGame::initShaders()
{
	//warning!!! order matters!!!
	terrainProgram.CompileShaders("shaders/terrainShading.vert","shaders/terrainShading.frag");
	terrainProgram.AddAttribute("vertexPosition");
	terrainProgram.AddAttribute("vertexNormal");
	terrainProgram.AddAttribute("vertexColor");
	terrainProgram.AddAttribute("vertexUV");
	terrainProgram.LinkShader();

	modelProgram.CompileShaders("shaders/modelShading.vert","shaders/modelShading.frag");
	modelProgram.AddAttribute("vertexPosition");
	modelProgram.AddAttribute("vertexNormal");
	modelProgram.AddAttribute("vertexColor");
	modelProgram.AddAttribute("vertexUV");
	modelProgram.LinkShader();
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
		//std::cout<<fps<<'\n';
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
		}
	}

	if(inputManager.IsKeyDown(SDLK_ESCAPE))
		gameState = EXIT;

	if(inputManager.IsKeyDown(SDLK_LSHIFT))
		CAMERA_SPEED=100.0f;
	else
		CAMERA_SPEED=1.1f;

	if(inputManager.IsKeyDownOnce(SDLK_F1))
		l->SwitchWireframeVisibility();
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

	//light movement
	static const float LIGHT_SPEED = 5.0f;
	if(inputManager.IsKeyDown(SDLK_UP))
		lightPos.z-=LIGHT_SPEED;
	if(inputManager.IsKeyDown(SDLK_DOWN))
		lightPos.z+=LIGHT_SPEED;
	if(inputManager.IsKeyDown(SDLK_LEFT))
		lightPos.x-=LIGHT_SPEED;
	if(inputManager.IsKeyDown(SDLK_RIGHT))
		lightPos.x+=LIGHT_SPEED;
	if(inputManager.IsKeyDown(SDLK_PAGEUP))
		lightPos.y+=LIGHT_SPEED;
	if(inputManager.IsKeyDown(SDLK_PAGEDOWN))
		lightPos.y-=LIGHT_SPEED;

	//std::cout<<lightPos.x<<" "<<lightPos.y<<" "<<lightPos.z<<"\n";

	if(SDL_GetRelativeMouseMode() == SDL_TRUE)
	{
		static const float MOUSE_SENSITIVITY = 0.2f;
		glm::vec2 mouseCoords = inputManager.GetMouseCoordsRel();
		camera.Rotate(glm::vec3(mouseCoords.y*MOUSE_SENSITIVITY, mouseCoords.x*MOUSE_SENSITIVITY,0.0f));
	}
	if(inputManager.IsKeyDown(SDL_BUTTON_LEFT))
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

		movement = objcoord;
	}
}

void MainGame::renderScene()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	terrainProgram.Use();	
	//move light
	GLint lightPosLocation = terrainProgram.GetUniformLocation("lightPos");
	glUniform3fv(lightPosLocation, 1, &lightPos[0]);
	//set the inverse matrix
// 	GLint inverseMatrixLocation = colorProgram.GetUniformLocation("inverseMatrix");
// 	glm::mat3 inverseMatrix = glm::mat3(glm::inverseTranspose(camera.GetViewMatrix()));
// 	glUniformMatrix3fv(inverseMatrixLocation, 1, GL_FALSE, &inverseMatrix[0][0]);

	//set the camera matrix(mvp)
	glm::mat4 cameraMatrix = camera.GetCameraMatrix();
	GLint mvpLocation = terrainProgram.GetUniformLocation("MVP");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	//set view matrix
// 	GLint mvLocation = colorProgram.GetUniformLocation("MV");
// 	glm::mat4 viewMatrix = camera.GetViewMatrix();
// 	glUniformMatrix4fv(mvLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	//actual drawing here
	l->Render();
	terrainProgram.UnUse();

	//move cube
	modelProgram.Use();
	lightPosLocation = modelProgram.GetUniformLocation("lightPos");
	glUniform3fv(lightPosLocation, 1, &lightPos[0]);
	//movement.x+=0.1;
	//movement.z+=0.1;
	movement.y=l->GetHeight(glm::vec2(movement.x, movement.z));
	cameraMatrix = glm::translate(cameraMatrix, movement);
	mvpLocation = modelProgram.GetUniformLocation("MVP");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	m->Render();
	modelProgram.UnUse();

	//glBindTexture(GL_TEXTURE_2D, 0);

	window.SwappBuffer();
}

MainGame::~MainGame(void)
{
}