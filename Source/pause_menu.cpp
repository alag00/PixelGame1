#include "pause_menu.h"

void PauseMenu::Setup(Camera2D& cam)
{
	camRef = &cam;
	settingsButton.Setup(0.f, 0.f, 200.f, 75.f, "Settings");
	skillTreeButton.Setup(0.f, 0.f, 200.f, 75.f, "Skills");
	backButton.Setup(0.f, 0.f, 200.f, 75.f, "Continue");

	settings.Setup();
	skillTree.Setup();
}

void PauseMenu::Activate()
{
	// Set Buttons pos based on camera
	Vector2 settingsPos = {-100.f, 100.f};
	Vector2 skillTreePos = { -100.f, 200.f };
	Vector2 backPos = { -100.f, 0.f };

	settingsPos = GetScreenToWorld2D(settingsPos, *camRef);
	skillTreePos = GetScreenToWorld2D(skillTreePos, *camRef);
	backPos = GetScreenToWorld2D(backPos, *camRef);

	settingsButton.x = settingsPos.x;
	settingsButton.y = settingsPos.y;

	skillTreeButton.x = skillTreePos.x;
	skillTreeButton.y = skillTreePos.y;

	backButton.x = backPos.x;
	backButton.y = backPos.y;
}

bool PauseMenu::Update()
{
	switch (currentMenu)
	{
	case MENU_TYPE::GENERAL:
		if (CheckButtons())
		{
			return true;
		}
		break;
	case MENU_TYPE::SETTINGS:
		if (settings.Update())
		{
			currentMenu = MENU_TYPE::GENERAL;
		}
		break;
	case MENU_TYPE::SKILLTREE:
		if (skillTree.Update())
		{
			currentMenu = MENU_TYPE::GENERAL;
		}
		break;
	}
	return false;
}

void PauseMenu::Render()
{
	switch (currentMenu)
	{
	case MENU_TYPE::GENERAL:
		RenderButtons();
		break;
	case MENU_TYPE::SETTINGS:
		settings.Render();
		break;
	case MENU_TYPE::SKILLTREE:
		skillTree.Render();
		break;
	}
}

bool PauseMenu::CheckButtons()
{
	Vector2 mousePos = GetMousePosition();//GetScreenToWorld2D(GetMousePosition(), *camRef);
	if (settingsButton.Update(mousePos))
	{
		currentMenu = MENU_TYPE::SETTINGS;
		settings.Activate(*camRef);
		return false;
	}
	if (skillTreeButton.Update(mousePos))
	{
		currentMenu = MENU_TYPE::SKILLTREE;
		skillTree.Activate(*camRef);
		return false;
	}
	if (IsKeyPressed(KEY_ESCAPE) || backButton.Update(mousePos))
	{
		return true;
	}
	return false;
}

void PauseMenu::RenderButtons()
{
	settingsButton.Render();
	skillTreeButton.Render();
	backButton.Render();
	DrawCircle(GetMouseX(), GetMouseY(), 10.f, RED);
	DrawText("PAUSED", static_cast<int>(backButton.x), static_cast<int>(backButton.y - 200), 40, YELLOW);
}
