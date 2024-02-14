
#include "hud.h"

Hud::Hud()
{
	healthBarTxr = LoadTexture("Resources/HealthBar.png");
	miniMapTxr = LoadTexture("Resources/Frame.png");
}

void Hud::UpdateHud(Camera2D cam)
{
	Vector2 vec{ 50.f, 50.f };
	hpX = GetScreenToWorld2D(vec, cam).x;
	hpY = GetScreenToWorld2D(vec, cam).y;
	
	vec = {config.screenWidth - (miniMapTxr.width * config.PIXEL_SCALE), 0.f};
	mapX = GetScreenToWorld2D(vec, cam).x;
	mapY = GetScreenToWorld2D(vec, cam).y;

	vec = {config.screenWidth / 2.f, config.screenHeight / 2.f };
	coordX = static_cast<int>(GetScreenToWorld2D(vec, cam).x);
	coordY = static_cast<int>(GetScreenToWorld2D(vec, cam).y);

	mouseCoordX = static_cast<int>(GetScreenToWorld2D(GetMousePosition(), cam).x);
	mouseCoordY = static_cast<int>(GetScreenToWorld2D(GetMousePosition(), cam).y);
}
void Hud::RenderHud()
{
	DrawRectangle(static_cast<int>(hpX), static_cast<int>(hpY),
		static_cast<int>(healthBarTxr.width * config.PIXEL_SCALE), 
		static_cast<int>(healthBarTxr.height * config.PIXEL_SCALE), RED);

	float rotation = 0.0f;
	Rectangle src = { 0.f, 0.f, static_cast<float>(healthBarTxr.width), static_cast<float>(healthBarTxr.height) };
	Rectangle dst = { hpX, hpY, static_cast<float>(healthBarTxr.width * config.PIXEL_SCALE), static_cast<float>(healthBarTxr.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(healthBarTxr.width / 2.0f), static_cast<float>(healthBarTxr.height / 2.0f) };

	DrawTexturePro(healthBarTxr, src, dst, origin, rotation, WHITE);

	DrawRectangle(static_cast<int>(mapX), static_cast<int>(mapY),
		static_cast<int>(miniMapTxr.width * config.PIXEL_SCALE),
		static_cast<int>(miniMapTxr.height * config.PIXEL_SCALE), GREEN);
	
	DrawText(TextFormat("X: %i", coordX), static_cast<int>(mapX), static_cast<int>(mapY), 50, BLACK);
	DrawText(TextFormat("Y: %i", coordY), static_cast<int>(mapX), static_cast<int>(mapY) + 50, 50, BLACK);

	DrawText(TextFormat("X: %i", mouseCoordX), static_cast<int>(mapX), static_cast<int>(mapY + 100), 25, BLACK);
	DrawText(TextFormat("Y: %i", mouseCoordY), static_cast<int>(mapX), static_cast<int>(mapY) + 125, 25, BLACK);
	/*
	 src = { 0.f, 0.f, static_cast<float>(miniMapTxr.width), static_cast<float>(miniMapTxr.height) };
	 dst = { mapX, mapY, static_cast<float>(miniMapTxr.width * config.PIXEL_SCALE), static_cast<float>(miniMapTxr.height * config.PIXEL_SCALE) };
	 origin = { static_cast<float>(miniMapTxr.width / 2.0f), static_cast<float>(miniMapTxr.height / 2.0f) };

	DrawTexturePro(miniMapTxr, src, dst, origin, rotation, WHITE);
	*/


}