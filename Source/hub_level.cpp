#include "hub_level.h"

void Hub::LoadScene(GameInfo gameInfo)  {
	m_gameInfo = gameInfo;
	//mainButton.Setup(0.f, config.screenHeight / 2.f, 200.f, 100.f, "Main Menu");
	//playButton.Setup((config.screenWidth / 2.f) + 200, config.screenHeight / 2.f, 200.f, 100.f, "Play");

	camera.target.x = config.screenWidth / 2.f;
	camera.target.y = config.screenHeight / 2.f;
	camera.zoom = 1.f;

	backgroundTxr = LoadTexture("Resources/Background.png");
	CreateWalls();

	if (gameInfo.playerClass != 0)
	{
		showClasses = false;
		SetPlayer();
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




	
	/*
	room.Setup(0.f ,0.f, backgroundTxr);
	room.width = static_cast<float>(config.screenWidth);
	room.height = static_cast<float>(config.screenHeight);
	room.FillContent();
	*/
	//Image testAtlas = LoadImage("Resources/Arcanist.png");
	//Texture2D atlas = LoadTexture("Resources/SpriteSheet.png");
	//anim.SetAnimation(atlas, 11, false);
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
				SetPlayer();	
			}
		}
		return false;
	}
	/*
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
	*/

	float deltaTime = GetFrameTime();
	player->Update(deltaTime);
	levelEntrance.Update();
	WallCollisionCheck();
	//room.CollisionCheck(player);
	if (levelEntrance.CheckActive())
	{
		ChangeToPlay();
		return true;
	}
	return false;
}

void Hub::RenderBackground()
{
	//Rectangle camRect = { 0.f, 0.f, static_cast<float>(config.screenWidth), static_cast<float>(config.screenHeight) };
	//room.Render(camRect);
	
	
	float rotation = 0.0f;

	Rectangle src = { 0.f, 0.f, static_cast<float>(backgroundTxr.width), static_cast<float>(backgroundTxr.height) };
	Rectangle dst = { 0.f, 0.f, static_cast<float>(config.screenWidth), static_cast<float>(config.screenHeight) };
	Vector2 origin = { 0.f, 0.f };

	DrawTexturePro(backgroundTxr, src, dst, origin, rotation, WHITE);
	/*
	Color color = RED;
//	color.a = 50;
	for (int i = 0; i < 4; i++)
	{
		DrawRectangleRec(collisionWalls.at(i), color);
	}
	*/
}

void Hub::Render()  {
	BeginDrawing();
	ClearBackground(BROWN);
	// Background
	RenderBackground();
	//DrawText("This is The Hub", config.screenWidth / 2, config.screenHeight / 2, 10, BLACK);
	if (!showClasses)
	{
		player->Render();
	}
	levelEntrance.Render();
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
	//playButton.Render();
	//mainButton.Render();

	//Rectangle dest{ 0.f, 0.f, 32 * config.PIXEL_SCALE, 32 * config.PIXEL_SCALE };
	//anim.DrawAnimationPro(dest, Vector2{ 0.f, 0.f }, 0, WHITE);
}

void Hub::SetPlayer()
{
	std::vector<EnemyCorpse> corpseList{};
	player = new Player(m_gameInfo, camera, corpseList);
	levelEntrance.SetPlayerRef(player);
	levelEntrance.MakeAvailable();
	player->SetPosition(camera.target);

	Vector2 entrancePos = { static_cast<float>(config.screenWidth - 100.f),static_cast<float>(config.screenHeight / 2.f) };
	levelEntrance.SetPosition(entrancePos);
}

void Hub::CreateWalls()
{
	float width = static_cast<float>(config.screenWidth);
	float height = static_cast<float>(config.screenHeight);
	float thickness = 50.f;
	Rectangle top{ 0.f, 0.f,width, thickness };
	collisionWalls.push_back(top);
	Rectangle bot{ 0.f, height - thickness,width, thickness };
	collisionWalls.push_back(bot);
	Rectangle left{ 0.f, 0.f,thickness, height };
	collisionWalls.push_back(left);
	Rectangle right{ width - thickness, 0.f,thickness, height };
	collisionWalls.push_back(right);
}

void Hub::WallCollisionCheck()
{
	for (int i = 0; i < collisionWalls.size(); i++)
	{
		if (player->AABBvsAABBCheck(collisionWalls.at(i)))
		{
			WallOnCollision(collisionWalls.at(i));
		}
	}
}

void Hub::WallOnCollision(Rectangle wall)
{
	if (wall.width > wall.height)
	{
		if (wall.y < player->y)
		{
			player->y = wall.y + wall.height;
			return;
		}
		player->y = wall.y - player->height;
		return;
	}

	if (wall.x < player->x)
	{
		player->x = wall.x + wall.width;
		return;
	}
	player->x = wall.x - player->width;
}
