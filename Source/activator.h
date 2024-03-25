#pragma once
#include "dynamic_entity.h"
#include <math.h>
#include "animation.h"
#include "config.h"
class Activator : public Entity
{
private:
	DynamicEntity* playerRef = nullptr;
	bool inRange = false;
	float range = 50.f;
	bool active = false;
	Texture2D portalAtlas{};
	Animator anim;
	Rectangle dst = { 0.f,0.f,0.f,0.f };
	Config config;
public:
	Activator()
	{
		portalAtlas = LoadTexture("Resources/TeleporterAtlas.png");
		anim.SetAnimation(portalAtlas, 3, true);
		height = static_cast<float>(portalAtlas.height * config.PIXEL_SCALE);
		width = static_cast<float>((portalAtlas.width / 3.f) * config.PIXEL_SCALE);

		dst.width = width;
		dst.height = height;

		range = width / 2.f;
	}
	void SetPosition(Vector2 newPos) override
	{
		Entity::SetPosition(newPos);
		dst.x = newPos.x - (width / 2.f);
		dst.y = newPos.y - (height / 2.f);
	}
	void SetPlayerRef(DynamicEntity* player) 
	{ 
		playerRef = player; 
	}
	void Update()
	{
		if (!active)
		{
			return;
		}
		inRange = false;

		float deltaX = x - playerRef->GetCenter().x;
		float deltaY = y - playerRef->GetCenter().y;

		float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

		if (distance <= range)
		{
			inRange = true;
		}
	}
	bool CheckActive()
	{
		if (!active)
		{
			return false;
		}
		if (inRange && IsKeyPressed(KEY_E))
		{
			return true;
		}
		return false;
	}
	void Render()
	{
		if (!active)
		{
			return;
		}
		Vector2 origin = { 0.f, 0.f };
		anim.DrawAnimationPro(dst, origin, 0.f, WHITE);
	}
	void RenderUI()
	{
		if (!active || !inRange)
		{
			return;
		}
		Rectangle speechBox = dst;
		speechBox.y -= speechBox.height;
		DrawRectangleRec(speechBox, WHITE);

		DrawText("Press 'E'", static_cast<int>(speechBox.x), static_cast<int>(speechBox.y), 25, BLACK);
	}
	void MakeAvailable() { active = true; }
};