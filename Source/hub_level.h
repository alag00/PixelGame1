#pragma once

#include "scene.h"
#include "config.h"
#include "button.h"
#include "animation.h"
#include "activator.h"
#include "player.h"
#include "corpse.h"
//#include "room.h"

class Hub : public Scene
{
private:
	Config config;
	GameInfo m_gameInfo;
	Button mainButton; // onClick = switch Scene to main menu
	Button playButton; // onClick = switch Scene to Level
	SCENE_TYPE nextScene = SCENE_TYPE::NONE;

	// Class Buttons
	static const short int classAmount = 6;
	Button classButtons[classAmount];
	bool showClasses = true;
	float buttonSize = 200.f;

	// Background
	Texture2D backgroundTxr{};
	//Animator anim;
	std::vector<Rectangle> collisionWalls{};

	Activator levelEntrance;
	Player* player = nullptr;
	Camera2D camera;
public:
	void LoadScene(GameInfo gameInfo) override;
	void LeaveScene(GameInfo& gameInfo) override {gameInfo = m_gameInfo;}
	bool Update() override;
	void RenderBackground();
	void Render() override;
	void RenderUI() override;
	void ChangeToPlay(){nextScene = SCENE_TYPE::DUNGEON_LEVEL;}
	void ChangeToMain(){nextScene = SCENE_TYPE::MAIN_MENU;}
	SCENE_TYPE GetNewScene() override{return nextScene;}
	void SetPlayer();
	void CreateWalls();
	void WallCollisionCheck();
	void WallOnCollision(Rectangle wall);
};

// First time in Hud = Choose your Class and Deity
// Have a button to return to main menu
// Have a button to go to level