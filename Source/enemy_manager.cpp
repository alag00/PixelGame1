#include "enemy_manager.h"


void EnemyManager::SetPlayerRef(DynamicEntity* player)
{
	playerRef = player;
}
void EnemyManager::CreateEnemies()
{
	// Randomize between all different enemies and have 2-3 be the only enemy types
	int enemyAmount = 10;
	for (int i = 0; i < enemyAmount; i++)
	{
		Specter* newEnemy = new Specter(*playerRef);
		enemyList.push_back(newEnemy);
	}
}
void EnemyManager::Update(float deltaTime)
{
	if (enemyList.size() > 0)
	{
		for (int i = 0; i < enemyList.size(); i++)
		{
			enemyList.at(i)->Update(deltaTime);
		}
	}
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