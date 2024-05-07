#pragma once

#include "raylib.h"
#include "config.h"
#include <cstring>
class Button : public Rectangle{
private:
	bool isHover = false;
	Config config;
	const char* text = "";
	int fontSize = 20;
	Color color = BLACK;
	Texture2D texture{};
	Texture2D bonusTexture{};
	bool hasNoTexture = true;

	bool hasToolTip = false;
	Rectangle toolTipBox{0.f,0.f,0.f,0.f};
	const char* toolTipText = "";
public:
	void Setup(float newPosX, float newPosY, float newWidth, float newHeight, const char* _text);
	void SetupImage(Texture2D buttonTexture);
	void SetupImage(Texture2D buttonTexture, Texture2D otherTexture);
	void SetupToolTip(const char* _text);
	bool Update(Vector2 mousePos);
	void Render();
	void Hover();
};