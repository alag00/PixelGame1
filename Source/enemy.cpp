#include "enemy.h"

void Enemy::Update(float deltaTime) 
{
	if (!GetIsActive())
	{
		return;
	}
	timer += deltaTime;
	if (timer >= RESPOND_TIME)
	{
		Sense();
		Decide();
		timer = 0.f;
	}
	Act(deltaTime);
	InvincibleCheck(deltaTime);
}

bool Enemy::IsAlive()
{
	if (GetHealth() <= 0)
	{
		return false;
	}
	return true;
}

void Enemy::Render()
{
	if (!GetIsActive())
	{
		return;
	}
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	float rotation = 0.0f;
	Color color = WHITE;
	if (GetInvincibility())
	{
		color = RED;
	}
	DrawTexturePro(texture, src, dst, origin, rotation, color);

	color = YELLOW;
	color.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

	// hp bar
	int barHeight = static_cast<int>(config.PIXEL_SCALE * 2);
	float hpPerc = static_cast<float>(GetHealth()) / static_cast<float>(GetMaxHealth());
	int barWidth = static_cast<int>(std::lerp(0.f, width, hpPerc));
	DrawRectangle(static_cast<int>(x), static_cast<int>(y + height), barWidth, barHeight, RED);
}

void Enemy::SetTexture(Texture2D txr)
{
	texture = txr;	
	width = texture.width * config.PIXEL_SCALE;
	height = texture.height * config.PIXEL_SCALE;
}

void Enemy::InFrameCheck(Rectangle cam)
{
	if (x  < cam.x + cam.width
		&& x + width  > cam.x
		&& y  < cam.y + cam.height
		&& y + height  > cam.y)
	{
		SetIsActive(true);
		return;
	}
	SetIsActive(false);
}

void Enemy::TakeDamage(int totalDmg)
{
	if (GetInvincibility())
	{
		return;
	}
	DynamicEntity::TakeDamage(totalDmg);
	SetInvincibility(true);
	invincibleTimer = invincibleTimeMax;
}

void Enemy::InvincibleCheck(float deltaTime)
{
	if (!GetInvincibility())
	{
		return;
	}
	invincibleTimer -= deltaTime;
	if (invincibleTimer <= 0.f)
	{
		SetInvincibility(false);
	}
}

// Imp
Imp::Imp(DynamicEntity& target)
{
	m_target = &target;
	bulletTxr = LoadTexture("Resources/Texture/FireBolt.png");
	basic.SetTexture(bulletTxr);
	SetHealth(15);
	SetMaxHealth(15);
	
}
void Imp::Sense() 
{
	if (m_target == nullptr)
	{
		return;
	}
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));
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
		basic.Activate(*this, m_target->GetCenter().x, m_target->GetCenter().y);
		shootTimer = SHOOT_COOLDOWN;
	}
}
void Imp::Render() 
{
	Enemy::Render();
	basic.Render();
}
void Imp::SpellCollisionCheck(DynamicEntity* other)
{
	basic.CollisionCheck(other);
}


// Crawler
Crawler::Crawler(DynamicEntity& target)
{
	m_target = &target;
	texture = LoadTexture("Resources/Texture/Crawler.png");
	SetHealth(5);
}

void Crawler::Sense()
{

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
	texture = LoadTexture("Resources/Texture/Specter.png");
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
	Enemy::Render();

}
