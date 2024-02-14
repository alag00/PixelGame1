#pragma once
#include "enemy.h"
#include "config.h"

// Holder of Textures
class EnemyManager
{
private:
	DynamicEntity* playerRef = nullptr;
	std::vector<Enemy*> enemyList;
	Config config;
public:
	void SetPlayerRef(DynamicEntity* player);
	void CreateEnemies();
	std::vector<Enemy*> GetEnemyList() { return enemyList; };
	void Update(float deltaTime);
	void Render();
	void ClearList();
	void SpawnEnemy();
	void DeleteEnemy();
	~EnemyManager();
};