#include "enemy_manager.h"


void EnemyManager::SetPlayerRef(DynamicEntity* player)
{
	playerRef = player;
}
void EnemyManager::CreateEnemies()
{
	// Randomize between all different enemies and have 2-3 be the only enemy types
	Texture2D enemyTxr = LoadTexture("Resources/Imp.png");
	int enemyAmount = 10;
	for (int i = 0; i < enemyAmount; i++)
	{
		Imp* newEnemy = new Imp(*playerRef);
		newEnemy->SetTexture(enemyTxr);
		enemyList.push_back(newEnemy);
	}
	enemyTxr = LoadTexture("Resources/ImpBoss.png");
	Boss* boss = new Boss(*playerRef);
	boss->SetTexture(enemyTxr);
	enemyList.push_back(boss);
}
bool EnemyManager::Update(float deltaTime)
{
	// check if enemies are alive

	if (enemyList.size() <= 0)
	{
		return false;
	}
	if (!enemyList.back()->IsAlive())
	{
		delete enemyList.back();
		enemyList.back() = nullptr;
		enemyList.erase(enemyList.begin() + enemyList.size()-1);
		return true;
	}

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (!enemyList.at(i)->IsAlive())
		{
			delete enemyList.at(i);
			enemyList.at(i) = nullptr;
			enemyList.erase(enemyList.begin() + i);
		}
	}

	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList.at(i)->Update(deltaTime);
	}
	return false;
}
void EnemyManager::Render()
{
	if (enemyList.size() > 0)
	{
		for (int i = 0; i < enemyList.size(); i++)
		{
			enemyList.at(i)->Render();
		}
	}
}
void EnemyManager::ClearList()
{
	if (enemyList.size() > 0)
	{
		for (int i = 0; i < enemyList.size(); i++)
		{
			delete enemyList.at(i);
			enemyList.at(i) = nullptr;
		}
	}
}
void EnemyManager::SpawnEnemy()
{
	Imp* newEnemy = new Imp(*playerRef);

	newEnemy->x = static_cast<float>(rand() % config.screenWidth);
	newEnemy->y = static_cast<float>(rand() % config.screenHeight);

	enemyList.push_back(newEnemy);

}

void EnemyManager::DeleteEnemy()
{
	delete enemyList.back();
	enemyList.erase(enemyList.begin() + (enemyList.size() - 1));
}
EnemyManager::~EnemyManager()
{
	ClearList();
}