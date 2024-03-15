#pragma once
#include "config.h"
#include "scene.h"
#include "button.h"
#include "animation.h"

class MainMenu : public Scene
{
private:
	Config config;
	GameInfo m_gameInfo;
	Button playButton; // onClick = switch Scene to Dungeon Level
	Button hubButton; // onClick = switch Scene to Dungeon Level
	SCENE_TYPE nextScene = SCENE_TYPE::NONE;

	//
	//Texture2D atlas{};
public:
	void LoadScene(GameInfo gameInfo) override;
	void LeaveScene(GameInfo& gameInfo) override{gameInfo = m_gameInfo;}
	bool Update() override;
	void Render() override;
	void RenderUI() override;

	void ChangeToPlay(){nextScene = SCENE_TYPE::DUNGEON_LEVEL;}
	void ChangeToHub(){nextScene = SCENE_TYPE::HUB;}
	SCENE_TYPE GetNewScene() override{return nextScene;}
};

// Stage 1: Background Image, Texts and functional Buttons that leads to DungeonLevel Scene
// Stage 2: Having a functional Settings Menu
// Stage 3: Having functional Saves which can be selected