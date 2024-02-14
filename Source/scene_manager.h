#pragma once
#include "scene.h"
#include "main_menu.h"
#include "level.h"
#include "hub_level.h"
#include "game_info.h"

class SceneManager {
private:
	Scene* currentScene = nullptr;
	GameInfo gameInfo;
public:
	SceneManager();
	~SceneManager();
	void Update();
	void ChangeScene(SCENE_TYPE sceneType);
	
};