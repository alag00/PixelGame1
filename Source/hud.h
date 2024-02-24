#pragma once
#include "raylib.h"
#include "config.h"
#include "player.h"
#include <math.h>
class Hud
{
private:
	Config config;
	
	Player* playerRef = nullptr;
	DynamicEntity* bossRef = nullptr;

	Texture2D healthBarTxr{};
	float hpX = 100.f;
	float hpY = 100.f;
	float hpW = 100.f;
	float hpH = 100.f;
	float barWidth = 0.f;
	float playerHpPrec = 1.f;

	Texture2D miniMapTxr{};
	float mapX = 100.f;
	float mapY = 100.f;

	int coordX = 100;
	int coordY = 100;

	int mouseCoordX = 100;
	int mouseCoordY = 100;

	Rectangle bossHpBar{100.f, 100.f, 400.f, 75.f};
	float currentBossWidth = 100.f;
	// Health
	// Mini Map
	// Some other stuff
	// maybe hotbar for abilities
public:
	Hud();
	void SetPlayerRef(Player* ref);
	void SetBossRef(DynamicEntity* ref);
	void UpdateHud(Camera2D cam);
	void RenderHud();
};