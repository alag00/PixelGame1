#pragma once
#include "entity.h"
#include <cmath>
 class DynamicEntity : public Entity
{
private:
	int health = 0;
public:
	float velX = 0.f;
	float velY = 0.f;
	

	virtual void Update(float deltaTime) {
		x += velX * deltaTime;
		y += velY * deltaTime;
		isColliding = false;
	}
	virtual void Render()
	{
		DrawCircle(static_cast<int>(x), static_cast<int>(y), 10.f, DARKBLUE);

		Color boxColor = YELLOW;
		if (isColliding)
		{
			boxColor = RED;
		}
		boxColor.a = 60;
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), boxColor);

	}
	void SetHealth(int newValue) { health = newValue; }
	int GetHealth() { return health; }
	void TakeDamage(int totalDmg) { health -= totalDmg; }
	void PushEntity(Vector2 force)
	{
		
		velX = force.x;
		velY = force.y;
	}
	Vector2 GetNormalizedVectorTowardsPos(float dstX, float dstY)
	{
		float vectorX = GetCenter().x - dstX;
		float vectorY = GetCenter().y - dstY;

		float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

		vectorX = (vectorX / length);
		vectorY = (vectorY / length);

		return Vector2{vectorX, vectorY};
	}
};