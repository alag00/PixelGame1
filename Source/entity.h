#pragma once
#include "raylib.h"
#include <cmath>
class Entity 
{
public:
	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;
	bool isColliding = false;
	// HitBox detection
	
	// Texture
	virtual void Render(){}
	virtual void OnCollision(Entity* other) 
	{ 
		isColliding = true; 
		other->x++;
		other->x--;
	}
	bool IsColliding(Entity other)
	{
		if (x < other.x + other.width && x + width > other.x &&
			y < other.y + other.height && y + height > other.y)
		{
			return true;
		}
		
		return false;
	}
	bool AABBvsAABBCheck(Rectangle rect) 
	{ 
		if (x < rect.x + rect.width && x + width > rect.x &&
			y < rect.y + rect.height && y + height > rect.y)
		{
			return true;
		}
		return false; 
	}
	bool AABBvsCircle(Vector2 pos, float radius) 
	{ 
		Vector2 nearestPoint;
		nearestPoint.x = std::max(x, std::min(x + width, pos.x));
		nearestPoint.y = std::max(y, std::min(y + height, pos.y));

		float dX = nearestPoint.x - pos.x;
		float dY = nearestPoint.y - pos.y;

		float distance = sqrt((dX * dX) + (dY * dY));
		if (distance <= radius)
		{
			return true;
		}
		return false;
	}

	virtual void SetPosition(Vector2 newPos)
	{
		x = newPos.x;
		y = newPos.y;
	}
	Vector2 GetCenter()
	{
		return Vector2{x+(width / 2.f), y + (height / 2.f) };
	}
};