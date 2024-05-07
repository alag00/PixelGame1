#include "settings_menu.h"

void SettingMenu::Setup()
{
	backButton.Setup(0.f, 0.f, 200.f, 75.f, "Back");
	audioButton.Setup(0.f, 0.f, 200.f, 75.f, "AUDIO");
	videoButton.Setup(0.f, 0.f, 200.f, 75.f, "VIDEO");

	audio.Setup();
	video.Setup();
}
void SettingMenu::Activate(Camera2D& cam)
{
	camRef = &cam;

	Vector2 backPos = { 0.f, 0.f };
	Vector2 audioPos = { 400.f, 0.f };
	Vector2 videoPos = { -400.f, 0.f };

	backPos = GetScreenToWorld2D(backPos, cam);
	audioPos = GetScreenToWorld2D(audioPos, cam);
	videoPos = GetScreenToWorld2D(videoPos, cam);

	backButton.x = backPos.x;
	backButton.y = backPos.y;

	audioButton.x = audioPos.x;
	audioButton.y = audioPos.y;

	videoButton.x = videoPos.x;
	videoButton.y = videoPos.y;
}
bool SettingMenu::Update()
{
	switch (currentMenu)
	{
	case SETTING_SECTION::GENERAL:
		if (UpdateButtons())
		{
			return true;
		}
		break;
	case SETTING_SECTION::AUDIO:
		if (audio.Update())
		{
			currentMenu = SETTING_SECTION::GENERAL;
		}
		break;
	case SETTING_SECTION::VIDEO:
		if (video.Update())
		{
			currentMenu = SETTING_SECTION::GENERAL;
		}
		break;
	}
	
	return false;
}
void SettingMenu::Render()
{
	switch (currentMenu)
	{
	case SETTING_SECTION::GENERAL:
		RenderGeneral();
		break;
	case SETTING_SECTION::AUDIO:
		audio.Render();
		break;
	case SETTING_SECTION::VIDEO:
		video.Render();
		break;
	}
	
}
bool SettingMenu::UpdateButtons()
{
	Vector2 mousePos = GetMousePosition();
	if (backButton.Update(mousePos))
	{
		return true;
	}
	if (audioButton.Update(mousePos))
	{
		audio.Activate(*camRef);
		currentMenu = SETTING_SECTION::AUDIO;
	}
	if (videoButton.Update(mousePos))
	{
		video.Activate(*camRef);
		currentMenu = SETTING_SECTION::VIDEO;
	}
	return false;
}
void SettingMenu::RenderGeneral()
{
	backButton.Render();
	audioButton.Render();
	videoButton.Render();
	DrawText("SETTINGS", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 200.f), 40, YELLOW);
}


// AUDIO SETTINGS
void AudioSettings::Setup()
{
	backButton.Setup(0.f, 0.f, 200.f, 75.f, "Back");

	
}
void AudioSettings::Activate(Camera2D cam)
{
	Vector2 centerPos = { 0.f, 0.f };

	centerPos = GetScreenToWorld2D(centerPos, cam);

	backButton.x = centerPos.x;
	backButton.y = centerPos.y + 300.f;

	masterSlider.Setup(true, centerPos.x + 200.f, centerPos.y - 200.f, 300.f, 10.f);
	vfxSlider.Setup(true, centerPos.x + 200.f, centerPos.y, 300.f, 10.f);
	musicSlider.Setup(true, centerPos.x + 200.f, centerPos.y + 200.f, 300.f, 10.f);

	float value = 0.5f;
	masterSlider.SetValue(value);
	vfxSlider.SetValue(value);
	musicSlider.SetValue(value);
}
bool AudioSettings::Update()
{
	Vector2 mousePos = GetMousePosition();
	if (backButton.Update(mousePos))
	{
		return true;
	}
	masterSlider.Update(mousePos);
	vfxSlider.Update(mousePos);
	musicSlider.Update(mousePos);

	return false;
}
void AudioSettings::Render()
{
	masterSlider.Render();
	vfxSlider.Render();
	musicSlider.Render();

	backButton.Render();
	DrawText("AUDIO", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 600.f), 40, YELLOW);
}


// VIDEO SETTINGS
void VideoSettings::Setup()
{
	backButton.Setup(0.f, 0.f, 200.f, 75.f, "Back");
}
void VideoSettings::Activate(Camera2D cam)
{
	Vector2 backPos = { 0.f, 0.f };

	backPos = GetScreenToWorld2D(backPos, cam);

	backButton.x = backPos.x;
	backButton.y = backPos.y;
}
bool VideoSettings::Update()
{
	Vector2 mousePos = GetMousePosition();
	if (backButton.Update(mousePos))
	{
		return true;
	}
	return false;
}
void VideoSettings::Render()
{
	backButton.Render();
	DrawText("VIDEO", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 200.f), 40, YELLOW);
}
