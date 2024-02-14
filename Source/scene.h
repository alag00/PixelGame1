#pragma once
#include "game_info.h"
enum class SCENE_TYPE
{
	NONE = 0,
	MAIN_MENU = 1,
	HUB = 2,
	DUNGEON_LEVEL = 3,
	//Etc....
};

// Main Menu, Dungeon Level, Shop Level
class Scene {
private:
public:
	virtual void LoadScene(GameInfo gameInfo) { gameInfo; };
	virtual bool Update() { return false; };
	virtual void Render() {};
	virtual void RenderUI() {};
	virtual SCENE_TYPE GetNewScene() { return SCENE_TYPE::NONE; };
	virtual void LeaveScene(GameInfo& gameInfo) { gameInfo; };
};

// Pause Menu, Settings, etc.. are not Scenes