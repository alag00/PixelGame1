#pragma once

#include "raylib.h"
#include "config.h"
#include "entity.h"
#include "dynamic_entity.h"
#include "player.h"
#include <vector>
#include <cstdlib>
#include "enemy_manager.h"
#include "scene.h"
#include "dungeon_generation.h"
#include "hud.h"

// Level = Scene
class Level : public Scene
{
private:
	Config config;
	GameInfo m_gameInfo;
	std::vector<DynamicEntity*> entityList; // stuffl ike enemies // should be in enemymanager
	std::vector<Entity*> staticEntityList; // stuff like walls
	EnemyManager enemyManager;
	Player* player = nullptr;
	//RoomManager roomManager;
	DungeonManager dungeonManager;
	Camera2D camera = {};
	Hud hud;

public:
	Level();
	~Level();
	void LoadScene(GameInfo gameInfo) override;// Generate Level
	void LeaveScene(GameInfo& gameInfo) override;// Leave Level // Return to Level Manager
	bool Update() override; // Update everything in the level // player, enemies, entities
	void UpdateEntities();
	void UpdateGameState();
	void CollisionCheck(); // Call in Update
	void UpdateCamera();
	void Render() override; // Render everything in the level // player, enemies, entities
	void RenderUI() override;

	
};