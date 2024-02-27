#include "level.h"

Level::Level()
{
	camera.target.x = 100.f;
	camera.target.y = 100.f;
	camera.zoom = 1.f;
}

Level::~Level()
{
	delete player;
	player = nullptr;

	
	enemyManager.ClearList();
	
}

void Level::LoadScene(GameInfo gameInfo)
{
	m_gameInfo = gameInfo;
	dungeonManager.GenerateDungeon();
	player = new Player(gameInfo, camera);
	player->SetPosition(dungeonManager.GetRoomList().front()->GetCenter());
	enemyManager.SetPlayerRef(player);
	enemyManager.CreateEnemies();
	dungeonManager.AddEnemiesToDungeon(enemyManager.GetEnemyList());
	hud.SetPlayerRef(player);
	hud.SetBossRef(enemyManager.GetEnemyList().back());
	levelExit.SetPlayerRef(player);
	cardManager.SetPlayerRef(player);
	cardManager.SetGameInfoRef(m_gameInfo);
}

void Level::LeaveScene(GameInfo& gameInfo)
{
	gameInfo = m_gameInfo;
	delete player;
	player = nullptr;

	enemyManager.ClearList();

}

bool Level::Update()
{
	// Debug
	if (IsKeyPressed(KEY_T))
	{
		player->SetPosition(dungeonManager.GetRoomList().back()->GetCenter());
	}


	UpdateEntities();
	CollisionCheck();
	UpdateCamera();
	return UpdateGameState();
}

void Level::UpdateEntities()
{
	float deltaTime = GetFrameTime();
	player->Update(deltaTime);

	Rectangle cameraBox{ camera.target.x , camera.target.y,
		static_cast<float>(config.screenWidth), static_cast<float>(config.screenHeight) };

	if (enemyManager.Update(deltaTime, cameraBox))
	{
		levelExit.SetPosition(dungeonManager.GetRoomList().back()->GetCenter());
		levelExit.MakeAvailable();
		cardManager.Activate();
	}
}

bool Level::UpdateGameState()
{
	levelExit.Update();
	if (levelExit.CheckActive())
	{
		nextScene = SCENE_TYPE::HUB;
		return true;
	}
	return false;
}

void Level::CollisionCheck()
{
	
	dungeonManager.CollisionCheck(player);
	for (int i = 0; i < enemyManager.GetEnemyList().size(); i++)
	{
		dungeonManager.CollisionCheck(enemyManager.GetEnemyList().at(i));
		player->SpellCollisionCheck(enemyManager.GetEnemyList().at(i));
		enemyManager.GetEnemyList().at(i)->SpellCollisionCheck(player);
	}

	

}
void Level::UpdateCamera()
{
	camera.target.x = player->GetCenter().x - (config.screenWidth / 2.f);
	camera.target.y = player->GetCenter().y - (config.screenHeight / 2.f);

	cardManager.Update(camera);
	hud.UpdateHud(camera);
}
void Level::Render()
{
	BeginDrawing();
	BeginMode2D(camera);
	ClearBackground(BLACK);

	Rectangle cameraBox{camera.target.x , camera.target.y, 
		static_cast<float>(config.screenWidth), static_cast<float>(config.screenHeight)};

	dungeonManager.Render(cameraBox);
	enemyManager.Render();
	
	player->Render();
	levelExit.Render();

	//Debug
	/*
	int x = static_cast<int>(cameraBox.x + 10);
	int y = static_cast<int>(cameraBox.y + 10);
	int width = static_cast<int>(cameraBox.width - 20);
	int height = static_cast<int>(cameraBox.height - 20);

	DrawLine(x, y, x + width, y, RED);
	DrawLine(x, y, x , y + height, RED);
	DrawLine(x + width, y + height, x - width, y + height, RED);
	DrawLine(x + width, y + height, x + width, y - height, RED);
	*/
	
	
	RenderUI();
	EndMode2D();
	EndDrawing();
}

void Level::RenderUI()
{
	hud.RenderHud();
	cardManager.Render();
}




