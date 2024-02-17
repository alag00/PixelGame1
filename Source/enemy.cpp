#include "enemy.h"

void Enemy::Update(float deltaTime) 
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

bool Enemy::IsAlive()
{
	if (GetHealth() <= 0)
	{
		return false;
	}
	return true;
}

// Imp
Imp::Imp(DynamicEntity& target)
{
	m_target = &target;
	//basic = new RangedBasicAttack();
	texture = LoadTexture("Resources/Imp.png");
	SetHealth(5);
}
void Imp::Sense() 
{
	if (m_target == nullptr)
	{
		return;
	}
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));// magnitude of vector from enemy to player
}
void Imp::Decide() 
{
	if (shootTimer > 0.f)
	{
		decision = DECISION::FLEE;
	}
	else if (distanceBetweenTarget > DETECTION_RANGE)
	{
		decision = DECISION::IDLE;
	}
	else if (distanceBetweenTarget > ATTACK_RANGE)
	{
		decision = DECISION::HUNT;
	}
	else if (distanceBetweenTarget <= ATTACK_RANGE)
	{
		decision = DECISION::ATTACK;
	}
	//std::cout << "Enemy: Decide Decision: " << static_cast<int>(decision) << std::endl;
	//std::cout << "Enemy: Own Position:  X = " << xPos << " | Y = " << yPos << std::endl;
	//std::cout << "Enemy: Target Position:  X = " << m_target->xPos << " | Y = " << m_target->yPos << std::endl;
}
void Imp::Act(float deltaTime) 
{
	if (m_target == nullptr)
	{
		return;
	}
	basic.Update(deltaTime);
	shootTimer -= deltaTime;
	if (decision == DECISION::FLEE || decision == DECISION::HUNT)
	{
		float vectorX = x - m_target->x;
		float vectorY = y - m_target->y;

		float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

		vectorX = (vectorX / length);
		vectorY = (vectorY / length);


		switch (decision)
		{
		case DECISION::FLEE:
			x += vectorX * deltaTime * fleeSpeed;
			y += vectorY * deltaTime * fleeSpeed;
			break;

		case DECISION::HUNT:
			x += -vectorX * deltaTime * huntSpeed;
			y += -vectorY * deltaTime * huntSpeed;
			break;
		}
	}
	else if (decision == DECISION::ATTACK)
	{
		// shoot projectile towards player
		basic.Activate(*this, m_target->x, m_target->y);
		shootTimer = SHOOT_COOLDOWN;
		//decision = DECISION::FLEE;
	}
	//std::cout << "Enemy: Act Decision: " << static_cast<int>(decision) << std::endl;
}
void Imp::Render() 
{
	//xPos = round(xPos);
	//yPos = round(yPos);
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(texture.width / 2.0f), static_cast<float>(texture.height / 2.0f) };
	float rotation = 0.0f;

	DrawTexturePro(texture, src, dst, origin, rotation, WHITE);

	//DrawTexture(texture, static_cast<int>(x), static_cast<int>(y), WHITE);
	//DrawCircle(static_cast<int>(xPos), static_cast<int>(yPos), size, RED);
	basic.Render();
}


// Crawler
Crawler::Crawler(DynamicEntity& target)
{
	m_target = &target;
	//basic = new RangedBasicAttack();
	texture = LoadTexture("Resources/Crawler.png");
	SetHealth(5);
}

void Crawler::Sense()
{
	/*
	if (m_target == nullptr)
	{
		return;
	}
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));// magnitude of vector from enemy to player
	*/
}

void Crawler::Decide()
{
}

void Crawler::Act(float deltaTime)
{
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = (vectorX / length);
	vectorY = (vectorY / length);




	x += -vectorX * deltaTime * speed;
	y += -vectorY * deltaTime * speed;


}

void Crawler::Render()
{
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(texture.width / 2.0f), static_cast<float>(texture.height / 2.0f) };
	float rotation = 0.0f;

	DrawTexturePro(texture, src, dst, origin, rotation, WHITE);
}

Specter::Specter(DynamicEntity& target)
{
	m_target = &target;
	//basic = new RangedBasicAttack();
	texture = LoadTexture("Resources/Specter.png");
	SetHealth(5);
	width = texture.width * config.PIXEL_SCALE;
	height = texture.height * config.PIXEL_SCALE;
}

void Specter::Sense()
{
}

void Specter::Decide()
{
}

void Specter::Act(float deltaTime)
{
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = (vectorX / length);
	vectorY = (vectorY / length);




	x += -vectorX * deltaTime * speed;
	y += -vectorY * deltaTime * speed;

}

void Specter::Render()
{
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(texture.width / 2.0f), static_cast<float>(texture.height / 2.0f) };
	float rotation = 0.0f;

	DrawTexturePro(texture, src, dst, origin, rotation, WHITE);

	//Enemy::Render();

}
