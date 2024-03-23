#include "hub_level.h"

void Hub::LoadScene(GameInfo gameInfo)  {
	m_gameInfo = gameInfo;
	mainButton.Setup(0.f, config.screenHeight / 2.f, 200.f, 100.f, "Main Menu");
	playButton.Setup((config.screenWidth / 2.f) + 200, config.screenHeight / 2.f, 200.f, 100.f, "Play");

	if (gameInfo.playerClass != 0)
	{
		showClasses = false;
		return;
	}

	buttonSize = config.screenWidth / 6.f;
	float xPos = (config.screenWidth - buttonSize) / 10.f;
	float yPos = (config.screenWidth - buttonSize) / 10.f;
	for (int i = 0; i < 3; i++)
	{

		classButtons[i].Setup(xPos, yPos, buttonSize, buttonSize, "");
		xPos += config.screenWidth / 3.f;
	}
	xPos = (config.screenWidth - buttonSize) / 10.f;
	yPos += config.screenHeight / 2.f;
	for (int i = 3; i < 6; i++)
	{
		classButtons[i].Setup(xPos, yPos, buttonSize, buttonSize, "");
		xPos += config.screenWidth / 3.f;
	}


	// Textures
	Texture2D frameTexture = LoadTexture("Resources/Frame.png"); // Button Texture
	Texture2D classTexture = LoadTexture("Resources/Arcanist.png");
	classButtons[0].SetupImage(frameTexture, classTexture);
	classButtons[0].SetupToolTip(" Arcanist | Ranged | Has Damage Dealing Balls Rotating Around You ");

	classTexture = LoadTexture("Resources/Summoner.png");
	classButtons[1].SetupImage(frameTexture, classTexture);
	classButtons[1].SetupToolTip(" Summoner | Can't Basic Attack | Can Summon Statues to Attack ");

	classTexture = LoadTexture("Resources/Necromancer.png");
	classButtons[2].SetupImage(frameTexture, classTexture);
	classButtons[2].SetupToolTip(" Necromancer | Ranged | Can Resurrect Fallen Enemies as Allies ");

	classTexture = LoadTexture("Resources/Enchanter.png");
	classButtons[3].SetupImage(frameTexture, classTexture);
	classButtons[3].SetupToolTip(" Enchanter | Melee, Versatile | Can Enchant Weapon ");

	classTexture = LoadTexture("Resources/Rogue.png");
	classButtons[4].SetupImage(frameTexture, classTexture);
	classButtons[4].SetupToolTip(" Rogue | Melee, Mobile | Can Dash");

	classTexture = LoadTexture("Resources/Paladin.png");
	classButtons[5].SetupImage(frameTexture, classTexture);
	classButtons[5].SetupToolTip(" Paladin | Melee, Slow, Healer, Tanky | Can Heal ");
	//for (int i = 0; i < 6; i++)
	//{
		//classButtons[i].SetupImage(frameTexture);
	//}
	//Texture2D arcanistTexture = LoadTexure("Resources/Arcanist.png"); // an entity with class Texture
	/*
	classButtons[0].Setup(0.f,0.f, buttonSize, buttonSize,	  "");
	classButtons[1].Setup(300.f, 0.f, buttonSize, buttonSize, "");
	classButtons[2].Setup(600.f, 0.f, buttonSize, buttonSize, "");
	classButtons[3].Setup(0.f, 200.f, buttonSize, buttonSize, "");
	classButtons[4].Setup(300.f, 200.f, buttonSize, buttonSize,"");
	classButtons[5].Setup(600.f, 200.f, buttonSize, buttonSize,"");
		*/

	backgroundTxr = LoadTexture("Resources/Background.png");


	//Image testAtlas = LoadImage("Resources/Arcanist.png");
	Texture2D atlas = LoadTexture("Resources/SpriteSheet.png");
	anim.SetAnimation(atlas, 11, false);
	//UnloadImage(testAtlas);
}
bool Hub::Update() {
	Vector2 mousePos = GetMousePosition();
	if (showClasses)
	{
		for (int i = 0; i < classAmount; i++)
		{
			if (classButtons[i].Update(mousePos))
			{
				m_gameInfo.playerClass = i + 1;
				showClasses = false;
			}
		}
		return false;
	}
	if (playButton.Update(mousePos))
	{
		ChangeToPlay();
		return true;
	}
	if (mainButton.Update(mousePos))
	{
		ChangeToMain();
		return true;
	}
	return false;
}
void Hub::RenderBackground()
{
	float rotation = 0.0f;

	Rectangle src = { 0.f, 0.f, static_cast<float>(backgroundTxr.width), static_cast<float>(backgroundTxr.height) };
	Rectangle dst = { (config.screenWidth / 2.f) - ((backgroundTxr.width * config.PIXEL_SCALE) / 2.f), 100.f, static_cast<float>(backgroundTxr.width * config.PIXEL_SCALE), static_cast<float>(backgroundTxr.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(backgroundTxr.width / 2.0f), static_cast<float>(backgroundTxr.height / 2.0f) };

	DrawTexturePro(backgroundTxr, src, dst, origin, rotation, WHITE);

}
void Hub::Render()  {
	BeginDrawing();
	ClearBackground(BROWN);
	// Background
	RenderBackground();
	DrawText("This is The Hub", config.screenWidth / 2, config.screenHeight / 2, 10, BLACK);



	RenderUI();
	EndDrawing();
}
void Hub::RenderUI()  {

	if (showClasses)
	{
		DrawRectangle(0, 0, config.screenWidth, config.screenHeight, BLACK);
		for (int i = 0; i < classAmount; i++)
		{
			classButtons[i].Render();
		}
		DrawText("CHOOSE YOUR CLASS", 50, 50, 50 ,YELLOW);
		return;
	}
	playButton.Render();
	mainButton.Render();

	Rectangle dest{ 0.f, 0.f, 32 * config.PIXEL_SCALE, 32 * config.PIXEL_SCALE };
	anim.DrawAnimationPro(dest, Vector2{ 0.f, 0.f }, 0, WHITE);
}