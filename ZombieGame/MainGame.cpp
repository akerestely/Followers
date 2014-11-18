#include "MainGame.h"

#include <Engine/Engine.h>
#include <Engine/Timing.h>

#include <SDL/SDL.h>
#include <iostream>

MainGame::MainGame(): screenWidth(600), screenHeight(400), gameState(PLAY)
{
    // Empty
}

MainGame::~MainGame() 
{
    for(int i=0;i<levels.size();i++)
		delete levels[i];
	levels.clear();
}

void MainGame::Run() 
{
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() 
{
    Engine::Init();

	window.Create("Zombie Game",screenWidth,screenHeight,0);
	glClearColor(0.7f,0.7f,0.7f,1.0f);

	initShaders();

	camera.Init(screenWidth, screenHeight);
}

void MainGame::initLevel()
{
	levels.push_back(new Level("Levels/level1.txt"));
	currentLevel=0;
}

void MainGame::initShaders() 
{
    // Compile our color shader
    textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    textureProgram.AddAttribute("vertexPosition");
    textureProgram.AddAttribute("vertexColor");
    textureProgram.AddAttribute("vertexUV");
    textureProgram.LinkShader();
}

void MainGame::gameLoop() 
{
	Engine::FpsLimiter fpsLimiter;
	fpsLimiter.SetMaxFps(60);

	while(gameState == PLAY)
	{
		fpsLimiter.Begin();

		processInput();
		camera.Update();

		drawGame();

		fps=fpsLimiter.End();
	}
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
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
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureProgram.Use();

    glActiveTexture(GL_TEXTURE0);

	GLint textureLocation=textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation,0);

	glm::mat4 projectionMatrix = camera.GetCameraMatrix();
	GLint pUniform = textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform,1, GL_FALSE, &projectionMatrix[0][0]);

	levels[currentLevel]->Draw();

	textureProgram.UnUse();
   
    // Swap our buffer and draw everything to the screen!
    window.SwappBuffer();
}