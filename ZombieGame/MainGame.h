#pragma once

#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Camera2D.h>
#include <Engine/InputManager.h>

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

enum GameState {PLAY,EXIT};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void Run();

private:
    /// Initializes the core systems
    void initSystems();

	void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

	//Update all agents
	void updateAgents(float deltaTime);

	//Update all bullets
	void updateBullets(float deltaTime);

    /// Handles input processing
    void processInput();

	void checkVictory();	

    /// Renders the game
    void drawGame();

    /// Member Variables
    Engine::Window window; ///< The game window
    
    Engine::GLSLProgram textureProgram; ///< The shader program

    Engine::InputManager inputManager; ///< Handles input

    Engine::Camera2D camera; ///< Main Camera
	Engine::SpriteBatch agentSpriteBatch;

	std::vector<Level *> levels;

	int screenWidth,screenHeight;

	float fps;

	int currentLevel;

	Player *player;
	std::vector<Human*> humans;
	std::vector<Zombie*> zombies;
	std::vector<Bullet> bullets;

	int numHumansKilled;
	int numZombiesKilled;

	GameState gameState;
};

