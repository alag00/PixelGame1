#pragma once
#include "raylib.h"

class Animator
{
private:
	int currentFrame = 0;
	int maxFrames = 0;
	Rectangle frameSize{ 0.f,0.f,0.f,0.f };
	Texture2D atlas{};
	float fps = 12.f;
	float frameTimer = 0.f;
public:
	void SetAnimation(Texture2D newSpriteSheet, int totalFrames);
	void DrawAnimationPro(Rectangle dest, Vector2 origin, float rotation, Color tint);

};