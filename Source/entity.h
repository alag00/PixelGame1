#pragma once
#include "raylib.h"
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
	void SetPosition(Vector2 newPos)
	{
		x = newPos.x;
		y = newPos.y;
	}
};