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
	// Create Map layout
	// Create Entities
	//roomManager.Setup();
	dungeonManager.GenerateDungeon();

	player = new Player(gameInfo.playerClass, camera);
	player->SetPosition(dungeonManager.GetRoomList().front()->GetCenter());
	enemyManager.SetPlayerRef(player);
	enemyManager.CreateEnemies();
	dungeonManager.AddEnemiesToDungeon(enemyManager.GetEnemyList());
	//hud.SetCamera(camera);
	hud.SetPlayerRef(player);
	
	
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
	UpdateEntities();
	CollisionCheck();
	UpdateGameState();
	UpdateCamera();
	return false;
}

void Level::UpdateEntities()
{
	float deltaTime = GetFrameTime();

	player->Update(deltaTime);

	enemyManager.Update(deltaTime);

	if (entityList.size() > 0)
	{
		for (int i = 0; i < entityList.size(); i++)
		{
			entityList.at(i)->Update(deltaTime);
		}
	}
	
}

void Level::UpdateGameState()
{

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
	camera.target.x = player->x - (config.screenWidth / 2.f);
	camera.target.y = player->y - (config.screenHeight / 2.f);

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
	RenderUI();
	EndMode2D();
	EndDrawing();
}

void Level::RenderUI()
{
	// Hud, Buttons and such
	hud.RenderHud();
}




