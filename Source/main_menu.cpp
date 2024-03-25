#include "main_menu.h"

void MainMenu::LoadScene(GameInfo gameInfo) 
{
	m_gameInfo = gameInfo;
	// Setup Buttons
	//playButton.Setup(100.f, config.screenHeight / 2.f, 200, 100, "Play");
	hubButton.Setup((config.screenWidth / 2.f) - 100.f, config.screenHeight - 150.f, 200.f, 50.f, "PLAY");

	//backgroundTxr = LoadTexture("Resources/MainMenuConcept.png");
}
bool MainMenu::Update() 
{
	if (!hasLoadedTxr)
	{
		TextureSetup();
	}
	Vector2 mousePos = GetMousePosition();
	/*
	if (playButton.Update(mousePos))
	{
		ChangeToPlay();
		return true;
	}
	*/
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
	ClearBackground(BLACK);
	DrawBackground();
	
	//DrawText("Main Menu", config.screenWidth / 2, config.screenHeight / 2, 10, BLACK);

	RenderUI();
	EndDrawing();
}
void MainMenu::RenderUI()  
{
	//playButton.Render();
	hubButton.Render();

	
}

void MainMenu::DrawBackground()
{
	float margin = 50.f;
	Rectangle src = { 0.f, 0.f , static_cast<float>(backgroundTxr.width),  static_cast<float>(backgroundTxr.height) };
	Rectangle dst = { 0.f, -margin,  static_cast<float>(config.screenWidth),  static_cast<float>(config.screenHeight + margin) };
	Vector2 origin = { 0.f,0.f };
	DrawTexturePro(backgroundTxr, src, dst, origin, 0.f, WHITE);
}
void MainMenu::TextureSetup()
{
	backgroundTxr = LoadTexture("Resources/MainMenuConcept.png");
	hasLoadedTxr = true;
}
