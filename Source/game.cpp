#include "game.h"

Game::Game()
{
	InitWindow(config.screenWidth, config.screenHeight, "PttA");
	//InitAudioDevice();
	SetTargetFPS(config.fpsTarget);
	Image icon = LoadImage("Resources/Texture/Necromancer.png");
	SetWindowIcon(icon);
	UnloadImage(icon);
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


