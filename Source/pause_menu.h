#pragma once

#include "settings_menu.h"
#include "skill_tree_menu.h"
#include "button.h"
#include "config.h"

enum MENU_TYPE
{
	GENERAL = 0,
	SETTINGS = 1,
	SKILLTREE = 2,
};
class PauseMenu
{
private:
	Config config;

	MENU_TYPE currentMenu = MENU_TYPE::GENERAL;
	SettingMenu settings;
	SkillTreeMenu skillTree;

	Button settingsButton;
	Button skillTreeButton;
	Button backButton;

	Camera2D* camRef = nullptr;
public:
	void Setup(Camera2D& cam);
	void Activate();
	bool Update();
	void Render();
	bool CheckButtons();

	void RenderButtons();

};