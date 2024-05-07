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
	
	Button hubButton; 
	SCENE_TYPE nextScene = SCENE_TYPE::NONE;

	
	bool hasLoadedTxr = false;
	Texture2D backgroundTxr{};
public:
	void LoadScene(GameInfo gameInfo) override;
	void LeaveScene(GameInfo& gameInfo) override{gameInfo = m_gameInfo;}
	bool Update() override;
	void Render() override;
	void RenderUI() override;

	void ChangeToPlay(){nextScene = SCENE_TYPE::DUNGEON_LEVEL;}
	void ChangeToHub(){nextScene = SCENE_TYPE::HUB;}
	SCENE_TYPE GetNewScene() override{return nextScene;}

	void DrawBackground();

	void TextureSetup();
};
