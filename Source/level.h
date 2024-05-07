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
#include "activator.h"
#include "card_manager.h"

// Level = Scene
class Level : public Scene
{
private:
	SCENE_TYPE nextScene = SCENE_TYPE::NONE;
	Config config;
	GameInfo m_gameInfo;
	EnemyManager enemyManager;
	Player* player = nullptr;
	DungeonManager dungeonManager;
	Camera2D camera = {};
	Hud hud;
	Activator levelExit;
	CardManager cardManager;
	int levelTheme = 0;
public:
	Level();
	~Level();
	void SetRandomTheme(GameInfo gameInfo);
	void LoadScene(GameInfo gameInfo) override;
	void LeaveScene(GameInfo& gameInfo) override;
	bool Update() override; 
	void UpdateEntities();
	bool UpdateGameState();
	void CollisionCheck(); 
	void UpdateCamera();
	void Render() override; 
	void RenderUI() override;
	SCENE_TYPE GetNewScene() override { return nextScene; }
	
};