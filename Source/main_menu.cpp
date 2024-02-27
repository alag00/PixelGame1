#include "main_menu.h"

void MainMenu::LoadScene(GameInfo gameInfo) 
{
	m_gameInfo = gameInfo;
	// Setup Buttons
	playButton.Setup(0.f, config.screenHeight / 2.f, 200, 100, "Play");
	hubButton.Setup((config.screenWidth / 2.f) + 200, config.screenHeight / 2.f, 200, 100, "Hub");
}
bool MainMenu::Update() 
{
	Vector2 mousePos = GetMousePosition();
	if (playButton.Update(mousePos))
	{
		ChangeToPlay();
		return true;
	}
	if (hubButton.Update(mousePos))
	{
		ChangeToHub();
		return true;
	}
	return false;
}
void MainMenu::Render()  
{
	BeginDrawing();
	ClearBackground(BROWN);
	DrawText("Main Menu", config.screenWidth / 2, config.screenHeight / 2, 10, BLACK);

	RenderUI();
	EndDrawing();
}
void MainMenu::RenderUI()  
{
	playButton.Render();
	hubButton.Render();
}