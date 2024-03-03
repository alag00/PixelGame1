#pragma once
#include "enemy.h"
#include "config.h"
#include "boss.h"
#include "Corpse.h"

// Holder of Textures
class EnemyManager
{
private:
	DynamicEntity* playerRef = nullptr;
	std::vector<Enemy*> enemyList;
	Config config;
	std::vector<EnemyCorpse> deadEnemies{};
public:
	void SetPlayerRef(DynamicEntity* player);
	void CreateEnemies();
	std::vector<Enemy*> GetEnemyList() { return enemyList; };
	std::vector<EnemyCorpse> &GetCorpseList() { return deadEnemies; };
	bool Update(float deltaTime, Rectangle cam);
	void Render();
	void ClearList();
	void SpawnEnemy();
	void DeleteEnemy();
	~EnemyManager();
};