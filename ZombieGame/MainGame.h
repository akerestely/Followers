#pragma once

#include <Engine/Window.h>
#include <Engine/GLSLProgram.h>
#include <Engine/Camera2D.h>
#include <Engine/InputManager.h>
#include "Level.h"

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

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Member Variables
    Engine::Window window; ///< The game window
    
    Engine::GLSLProgram textureProgram; ///< The shader program

    Engine::InputManager inputManager; ///< Handles input

    Engine::Camera2D camera; ///< Main Camera

	std::vector<Level *> levels;

	int screenWidth,screenHeight;

	float fps;

	int currentLevel;

	GameState gameState;
};

