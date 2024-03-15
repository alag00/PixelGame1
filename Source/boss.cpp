#include "boss.h"

Boss::Boss(DynamicEntity& target)
{
	m_target = &target;
	SetHealth(150);
	SetMaxHealth(150);
	walkAtlas = LoadTexture("Resources/ImpBossWalkAtlas.png");
	anim.SetAnimation(texture, 1);
}

void Boss::Sense()
{
	if (firstPhase && GetHealth() <= 50)
	{
		NextPhase();
		return;
	}
	if (m_target == nullptr)
	{
		return;
	}
	float vectorX = x - m_target->x;
	float vectorY = y - m_target->y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));
}

void Boss::Decide()
{
	

	if (attackCooldown > 0.f)
	{
		dec = BOSS_DECISION::IDLE;
		anim.SetAnimation(texture, 1);
	}
	else if (distanceBetweenTarget > attackRange)
	{
		dec = BOSS_DECISION::APPROACH;
		anim.SetAnimation(walkAtlas, 8);
	}
	else if (distanceBetweenTarget <= attackRange)
	{
		dec = BOSS_DECISION::ATTACK;
		anim.SetAnimation(texture, 1);
	}
}

void Boss::Act(float deltaTime)
{
	attackCooldown -= deltaTime;
	switch (dec)
	{
	case BOSS_DECISION::APPROACH:
		Approach(deltaTime);
		break;
	case BOSS_DECISION::ATTACK:
		Attack(deltaTime);
		break;
	}
}

void Boss::SpellCollisionCheck(DynamicEntity* other)
{
	if (!attackActive)
	{
		return;
	}
	PushPlayer();
	attackActive = false;
	other;
}

void Boss::Render()
{

	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	float rotation = 0.0f;

	anim.DrawAnimationPro(dst, origin, rotation, WHITE);
	//DrawTexturePro(texture, src, dst, origin, rotation, RED);

	

	Color color = WHITE;
	switch (dec)
	{
	case BOSS_DECISION::APPROACH:
		color = YELLOW;
		break;
	case BOSS_DECISION::ATTACK:
		color = RED;
		break;
	}
	color.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);
	
	/*
	if (dec == BOSS_DECISION::ATTACK)
	{
		Color color = RED;
		color.a = 50;
		//DrawRectangle(x, y, width, height, color);
	}*/
}

void Boss::NextPhase()
{
	baseSpeed *= 1.5f;
	chargeTime = 1.f;
	chargeCounter = chargeTime;
	
	PushPlayer();
	firstPhase = false;
}

void Boss::PushPlayer()
{

	Vector2 force = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	float pushPower = 15.f;
	force.x *= -pushPower;
	force.y *= -pushPower;

	//Vector2 force2 = {-vectorX, -vectorY};
	m_target->PushEntity(force);

	dec = BOSS_DECISION::IDLE;
	chargeCounter = chargeTime;
	attackCooldown = 3.f;
}

void Boss::Attack(float deltaTime)
{
	chargeCounter -= deltaTime;
	if (chargeCounter <= 0.f)
	{
		attackActive = true;
		chargeCounter = chargeTime;
		attackCooldown = 3.f;
	}
}

void Boss::Approach(float deltaTime)
{
	Vector2 dir = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	x += -dir.x * deltaTime * baseSpeed;
	y += -dir.y * deltaTime * baseSpeed ;
}
