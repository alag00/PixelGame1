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

	// delete theBugMan;
	// theBugMan = nullptr;
	if (entityList.size() > 0)
	{
		for (int i = 0; i < entityList.size(); i++)
		{
			delete entityList.at(i);
			entityList.at(i) = nullptr;
		}
	}

	enemyManager.ClearList();

	if (staticEntityList.size() > 0)
	{
		for (int i = 0; i < staticEntityList.size(); i++)
		{
			delete staticEntityList.at(i);
			staticEntityList.at(i) = nullptr;
		}
	}
}

void Level::LoadScene(GameInfo gameInfo)
{
	m_gameInfo = gameInfo;
	dungeonManager.GenerateDungeon();
	player = new Player(gameInfo.playerClass, camera);
	player->SetPosition(dungeonManager.GetRoomList().front()->GetCenter());
	enemyManager.SetPlayerRef(player);
	enemyManager.CreateEnemies();
	dungeonManager.AddEnemiesToDungeon(enemyManager.GetEnemyList());
	hud.SetPlayerRef(player);
	levelExit.SetPlayerRef(player);
	
}

void Level::LeaveScene(GameInfo& gameInfo)
{
	gameInfo = m_gameInfo;
	delete player;
	player = nullptr;

	for (int i = 0; i < entityList.size(); i++)
	{
		delete entityList.at(i);
		entityList.at(i) = nullptr;
	}

	enemyManager.ClearList();

	for (int i = 0; i < staticEntityList.size(); i++)
	{
		delete staticEntityList.at(i);
		staticEntityList.at(i) = nullptr;
	}

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

	if (enemyManager.Update(deltaTime))
	{
		levelExit.SetPosition(dungeonManager.GetRoomList().back()->GetCenter());
		levelExit.MakeAvailable();
	}

	if (entityList.size() > 0)
	{
		for (int i = 0; i < entityList.size(); i++)
		{
			entityList.at(i)->Update(deltaTime);
		}
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

	hud.UpdateHud(camera);
}
void Level::Render()
{
	BeginDrawing();
	BeginMode2D(camera);
	ClearBackground(BLACK);

	dungeonManager.Render();
	
	if (staticEntityList.size() > 0)
	{
		for (int i = 0; i < staticEntityList.size(); i++)
		{
			staticEntityList.at(i)->Render();
		}
	}

	enemyManager.Render();

	if (entityList.size() > 0)
	{
		for (int i = 0; i < entityList.size(); i++)
		{
			entityList.at(i)->Render();
		}
	}
	
	player->Render();
	levelExit.Render();
	RenderUI();
	EndMode2D();
	EndDrawing();
}

void Level::RenderUI()
{
	// Hud, Buttons and such
	hud.RenderHud();
}




