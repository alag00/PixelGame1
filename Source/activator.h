#pragma once
#include "dynamic_entity.h"
#include <math.h>
class Activator : public Entity
{
private:
	DynamicEntity* playerRef = nullptr;
	bool inRange = false;
	float range = 50.f;
	bool active = false;
public:
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
		DrawCircle(static_cast<int>(x), static_cast<int>(y), range, WHITE);
	}
	void MakeAvailable() { active = true; }
};