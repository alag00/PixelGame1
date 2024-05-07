#pragma once

#include "button.h"
#include "slider.h"


class AudioSettings
{
private:
	Slider masterSlider;
	Slider vfxSlider;
	Slider musicSlider;
	Button backButton;
	//Audio Sliders: Master, VFX, Music
public:
	void Setup();
	void Activate(Camera2D cam);
	bool Update();
	void Render();
};

class VideoSettings
{
	// Resolution
private:
	Button backButton;
public:
	void Setup();
	void Activate(Camera2D cam);
	bool Update();
	void Render();
};

class SettingMenu 
{
private:
	Camera2D* camRef = nullptr;
	Button backButton;
	Button audioButton;
	Button videoButton;
	AudioSettings audio;
	VideoSettings video;
	enum SETTING_SECTION
	{
		GENERAL = 0,
		AUDIO = 1,
		VIDEO = 2,
	};
	SETTING_SECTION currentMenu = SETTING_SECTION::GENERAL;
public:
	void Setup();
	void Activate(Camera2D& cam);
	bool Update();
	void Render();
	bool UpdateButtons();
	void RenderGeneral();
};
