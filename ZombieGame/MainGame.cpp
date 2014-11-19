#include "MainGame.h"

#include <Engine/Engine.h>
#include <Engine/Timing.h>
#include <Engine/Errors.h>

#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;

MainGame::MainGame(): screenWidth(1000), screenHeight(800), gameState(PLAY), player(nullptr), numHumansKilled(0), numZombiesKilled(0)
{
    // Empty
}

MainGame::~MainGame() 
{
    for(int i=0;i<levels.size();i++)
		delete levels[i];
	levels.clear();

	for(int i=0;i<humans.size();i++)
		delete humans[i];
	humans.clear();

	for(int i=0;i<zombies.size();i++)
		delete zombies[i];
	zombies.clear();
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

	agentSpriteBatch.Init();

	camera.Init(screenWidth, screenHeight);
}

void MainGame::initLevel()
{
	levels.push_back(new Level("Levels/level1.txt"));
	currentLevel=0;

	player = new Player();
	player->Init(4.0f, levels[currentLevel]->GetPlayerStartPos(), &inputManager, &camera, &bullets);
	
	humans.push_back(player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(0));
	std::uniform_int_distribution<int> randX(2, levels[currentLevel]->GetWidth() - 2);
	std::uniform_int_distribution<int> randY(2, levels[currentLevel]->GetHeight() - 2);

	//Add all the random humans
	for(int i=0; i<levels[currentLevel]->GetNumHumans();i++)
	{
		humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		humans.back()->Init(HUMAN_SPEED, pos);
	}

	//Add the starting zombies
	const std::vector<glm::vec2> &zombiePositions = levels[currentLevel]->GetZombieStartPos();
	for(int i=0; i<zombiePositions.size(); i++)
	{
		zombies.push_back(new Zombie);
		zombies.back()->Init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//set up the players guns
	player->AddGun(new Gun("Magnum", 10, 1, 10.0f, 30, 20.0f));
	player->AddGun(new Gun("Shotgun", 30, 12, 20.0f, 4, 20.0f));
	player->AddGun(new Gun("MP5", 2, 1, 10.0f, 10, 20.0f));
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
	const float DESIRED_FPS = 60;
	const int MAX_PHYSICS_STEPS = 6;

	Engine::FpsLimiter fpsLimiter;
	fpsLimiter.SetMaxFps(60000);

	const float CAMERA_SCALE = 1.0/4.0;
	camera.SetScale(CAMERA_SCALE);

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0;

	float prevTicks = SDL_GetTicks();

	while(gameState == PLAY)
	{
		fpsLimiter.Begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		processInput();

		int i = 0;
		while(totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		camera.SetPosition(player->GetPosition());
		camera.Update();

		drawGame();

		fps=fpsLimiter.End();

		std::cout<<fps<<'\n';
	}
}

void MainGame::updateAgents(float deltaTime)
{
	//update all humans
	for (int i=0; i<humans.size(); i++)
		humans[i]->Update(levels[currentLevel]->GetLevelData(),humans,zombies,deltaTime);

	//update all zombies
	for (int i=0; i<zombies.size(); i++)
		zombies[i]->Update(levels[currentLevel]->GetLevelData(),humans,zombies,deltaTime);

	//update zombies collisions
	for (int i=0; i<zombies.size(); i++)
	{
		//collide with other zombies
		for (int j=i+1; j<zombies.size(); j++)
		{
			zombies[i]->CollideWithAgent(zombies[j]);
		}
		//collide with humans
		for (int j=1; j<humans.size(); j++)
		{
			if(zombies[i]->CollideWithAgent(humans[j]))
			{
				zombies.push_back(new Zombie);
				zombies.back()->Init(ZOMBIE_SPEED,humans[j]->GetPosition());
				//delete the human
				delete humans[j];
				humans[j] = humans.back();
				humans.pop_back();
			}
		}
		if(zombies[i]->CollideWithAgent(player))
			Engine::fatalError("You lost!");
	}

	//update human collisions
	for (int i=0; i<humans.size()-1; i++)
	{
		for (int j=i+1; j<humans.size(); j++)
		{
			humans[i]->CollideWithAgent(humans[j]);
		}
	}
}

void MainGame::updateBullets(float deltaTime)
{
	// update & collide with world
	for (int i=0; i<bullets.size();)
	{
		if(bullets[i].Update(levels[currentLevel]->GetLevelData(),deltaTime))
		{
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
		else
			i++;
	}

	bool wasBulletRemoved;
	// collide with agents
	for (int i=0; i<bullets.size(); i++)
	{
		wasBulletRemoved = false;
		// check for zombies collision
		for(int j=0; j< zombies.size(); )
			if(bullets[i].CollideWithAgent(zombies[j]))
			{
				//damage zombies
				if(zombies[j]->ApplyDamage(bullets[i].GetDamage()))
				{
					delete zombies[j];
					zombies[j] = zombies.back();
					zombies.pop_back();
					numZombiesKilled++;
				}
				else
					j++;

				//remove the bullet
				bullets[i] = bullets.back();
				bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			}
			else
				j++;

		if(!wasBulletRemoved)
		{
			// check for humans collision
			for(int j=1; j< humans.size(); )
				if(bullets[i].CollideWithAgent(humans[j]))
				{
					//damage humans
					if(humans[j]->ApplyDamage(bullets[i].GetDamage()))
					{
						delete humans[j];
						humans[j] = humans.back();
						humans.pop_back();
						numHumansKilled++;
					}
					else
						j++;

					//remove the bullet
					bullets[i] = bullets.back();
					bullets.pop_back();
					i--;
					break;
				}
				else
					j++;
		}
	}
}

void MainGame::checkVictory()
{
	if (zombies.empty())
	{
		//TODO Support for multiple level. currentLevel++
		printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining.",
			numHumansKilled, numZombiesKilled, humans.size() -1, levels[currentLevel]->GetNumHumans());
		Engine::fatalError("");
	}
}


void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
			case SDL_QUIT:
				gameState=EXIT;
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

	agentSpriteBatch.Begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//draw the humans
	for (int i=0; i<humans.size(); i++)
		if(camera.IsBoxInView(humans[i]->GetPosition(),agentDims))
			humans[i]->Draw(agentSpriteBatch);

	//draw zombies
	for (int i=0; i<zombies.size(); i++)
		if(camera.IsBoxInView(zombies[i]->GetPosition(),agentDims))
			zombies[i]->Draw(agentSpriteBatch);

	//draw bullets
	for (int i=0; i<bullets.size(); i++)
		bullets[i].Draw(agentSpriteBatch);

	agentSpriteBatch.End();
	agentSpriteBatch.RenderBatches();

	textureProgram.UnUse();
   
    // Swap our buffer and draw everything to the screen!
    window.SwappBuffer();
}