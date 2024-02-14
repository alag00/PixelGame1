#pragma once
#include "raylib.h"
#include "config.h"
#include "scene_manager.h"

class Game
{
private:
	Config config;
	SceneManager sceneManager;
public:
	Game();
	~Game();
	void Run();
	void Update();
};