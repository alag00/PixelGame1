#include "game.h"

Game::Game()
{
	InitWindow(config.screenWidth, config.screenHeight, "PttA");
	SetTargetFPS(config.fpsTarget);
	Image icon = LoadImage("Resources/Necromancer.png");
	SetWindowIcon(icon);
}

Game::~Game()
{
	
}

void Game::Update()
{
	sceneManager.Update();
}

void Game::Run() 
{
	while (!WindowShouldClose())
	{
		Update();
	}
	CloseWindow();
}


