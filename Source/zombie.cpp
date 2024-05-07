#include "zombie.h"

void Zombie::Setup(Texture2D newTxr, Vector2 newPos, DynamicEntity* player)
{
	txr = newTxr;
	width = newTxr.width * config.PIXEL_SCALE;
	height = newTxr.height * config.PIXEL_SCALE;
	x = newPos.x - (width / 2.f);
	y = newPos.y - (height / 2.f);
	playerRef = player;
}

void Zombie::Sense()
{
	/*
		Get Distance between Player and Zombie
		Get Distance between Player and Closest Enemy (to player)
		Get Distance between Zombie and Closest Enemy (to zombie)
	*/
	distZtoP = GetDistance(GetCenter(), playerRef->GetCenter());
	
}

void Zombie::Decide()
{
	/*
	if			Player too far from zombie // Go to Player
	else if		Enemy too close to player // Target Enemy
	else		Target Closest Enemy to Zombie
	*/
	dec = ZOMBIE_DECISION::FOLLOW;
}

void Zombie::Act(float deltaTime)
{
	deltaTime;
	switch (dec)
	{
	case ZOMBIE_DECISION::FOLLOW:
		MoveTowards(playerRef->GetCenter());
		break;
	case ZOMBIE_DECISION::HUNT:
		// Move towards target
		break;
	case ZOMBIE_DECISION::ATTACK:
		// Attack target
		break;
	}
}

void Zombie::Update(float deltaTime)
{
	timer += deltaTime;
	if (timer >= RESPOND_TIME)
	{
		Sense();
		Decide();
		timer = 0.f;
	}
	Act(deltaTime);
}

void Zombie::Render()
{
	Rectangle src = { 0.f, 0.f, static_cast<float>(txr.width), static_cast<float>(txr.height) };
	Rectangle dst = { x, y, static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	
	float rotation = 0.0f;

	DrawTexturePro(txr, src, dst, origin, rotation, BLACK);
}

float Zombie::GetDistance(Vector2 vec1, Vector2 vec2)
{
	float deltaX = vec1.x - vec2.x;
	float deltaY = vec1.y - vec2.y;
	
	return static_cast<float>(sqrt((deltaX * deltaX)+(deltaY*deltaY)));
}

void Zombie::MoveTowards(Vector2 pos)
{
	Vector2 dir = GetNormalizedVectorTowardsPos(pos.x, pos.y);
	x -= dir.x * speed;
	y -= dir.y * speed;
}
