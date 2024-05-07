#pragma once
#include "button.h"

class SkillTreeMenu 
{
private:
	Button backButton;
	static const int skills = 10;
	Button skillButtons[skills];
public:
	void Setup();
	void Activate(Camera2D cam);
	bool Update();
	void Render();
	
};