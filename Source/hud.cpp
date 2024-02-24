
#include "hud.h"

Hud::Hud()
{
	healthBarTxr = LoadTexture("Resources/HealthBar.png");
	hpW = healthBarTxr.width * config.PIXEL_SCALE;
	hpH = healthBarTxr.height * config.PIXEL_SCALE;

	miniMapTxr = LoadTexture("Resources/Frame.png");
}

void Hud::SetPlayerRef(Player* ref)
{
	playerRef = ref;
}

void Hud::SetBossRef(DynamicEntity* ref)
{
	bossRef = ref;
}

void Hud::UpdateHud(Camera2D cam)
{
	// PLAYER HP
	playerHpPrec = static_cast<float>(playerRef->GetHealth()) / static_cast<float>(playerRef->GetMaxHealth());
	Vector2 vec{ 50.f, 50.f };
	hpX = GetScreenToWorld2D(vec, cam).x;
	hpY = GetScreenToWorld2D(vec, cam).y;
	
	// MAP
	vec = {config.screenWidth - (miniMapTxr.width * config.PIXEL_SCALE), 0.f};
	mapX = GetScreenToWorld2D(vec, cam).x;
	mapY = GetScreenToWorld2D(vec, cam).y;

	// PLAYER COORDS
	vec = {config.screenWidth / 2.f, config.screenHeight / 2.f };
	coordX = static_cast<int>(GetScreenToWorld2D(vec, cam).x);
	coordY = static_cast<int>(GetScreenToWorld2D(vec, cam).y);

	// MOUSE COORDS
	mouseCoordX = static_cast<int>(GetScreenToWorld2D(GetMousePosition(), cam).x);
	mouseCoordY = static_cast<int>(GetScreenToWorld2D(GetMousePosition(), cam).y);

	// BOSS HP
	vec = { (config.screenWidth / 2.f) - (bossHpBar.width / 2.f), config.screenHeight - (bossHpBar.height + 10.f)};
	bossHpBar.x = GetScreenToWorld2D(vec, cam).x;
	bossHpBar.y = GetScreenToWorld2D(vec, cam).y;
	float bossHpPrec = static_cast<float>(bossRef->GetHealth()) / static_cast<float>(bossRef->GetMaxHealth());
	currentBossWidth = std::lerp(0.f, bossHpBar.width, bossHpPrec);
}
void Hud::RenderHud()
{
	// Hp
	DrawRectangle(static_cast<int>(hpX), static_cast<int>(hpY), static_cast<int>(std::lerp(0.f, hpW, playerHpPrec)), static_cast<int>(hpH), RED);

	// HpBar
	float rotation = 0.0f;
	Rectangle src = { 0.f, 0.f, static_cast<float>(healthBarTxr.width), static_cast<float>(healthBarTxr.height) };
	Rectangle dst = { hpX, hpY, hpW, hpH };
	Vector2 origin = { 0.f, 0.f };
	
	DrawTexturePro(healthBarTxr, src, dst, origin, rotation, WHITE);
	DrawText(TextFormat("Health: %i", playerRef->GetHealth()), static_cast<int>(hpX), static_cast<int>(hpY), 25, BLACK);

	// Map
	DrawRectangle(static_cast<int>(mapX), static_cast<int>(mapY),
		static_cast<int>(miniMapTxr.width * config.PIXEL_SCALE),
		static_cast<int>(miniMapTxr.height * config.PIXEL_SCALE), GREEN);
	
	// Player Coords
	DrawText(TextFormat("X: %i", coordX), static_cast<int>(mapX), static_cast<int>(mapY), 50, BLACK);
	DrawText(TextFormat("Y: %i", coordY), static_cast<int>(mapX), static_cast<int>(mapY) + 50, 50, BLACK);

	// Mouse Coords
	DrawText(TextFormat("X: %i", mouseCoordX), static_cast<int>(mapX), static_cast<int>(mapY + 100), 25, BLACK);
	DrawText(TextFormat("Y: %i", mouseCoordY), static_cast<int>(mapX), static_cast<int>(mapY) + 125, 25, BLACK);

	// Boss Hp
	DrawRectangle(static_cast<int>(bossHpBar.x), static_cast<int>(bossHpBar.y), static_cast<int>(currentBossWidth) , static_cast<int>(bossHpBar.height), RED);
}