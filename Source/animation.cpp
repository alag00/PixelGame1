#include "animation.h"

void Animator::SetAnimation(Texture2D newSpriteSheet, int totalFrames)
{
	if (atlas.id == newSpriteSheet.id)
	{
		return;
	}
	atlas = newSpriteSheet;
	maxFrames = totalFrames;
	frameSize.height = static_cast<float>(atlas.height);
	frameSize.width = static_cast<float>(atlas.width / totalFrames);
	currentFrame = 0;
}

void Animator::DrawAnimationPro(Rectangle dest, Vector2 origin, float rotation, Color tint)
{

	frameSize.x = frameSize.width * currentFrame;
	frameSize.y = 0.f;
	DrawTexturePro(atlas, frameSize, dest, origin, rotation, tint);

	frameTimer += GetFrameTime();
	if (frameTimer >= 1.f / fps)
	{
		frameTimer = 0.f;
		currentFrame++;
		if (currentFrame >= maxFrames)
		{
			currentFrame = 0;
		}
	}
}
