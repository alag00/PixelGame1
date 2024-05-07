#include "boss_imp.h"

ImpBoss::ImpBoss(DynamicEntity& target)
{
	m_target = &target;
	SetHealth(150);
	SetMaxHealth(150);
	Texture2D tempTxr = LoadTexture("Resources/Texture/ImpBoss.png");
	SetTexture(tempTxr);

	walkAtlas = LoadTexture("Resources/Texture/ImpBossWalkAtlas.png");
	anim.SetAnimation(texture, 1, false);

	if (m_target->GetCenter().x < GetCenter().x)
	{
		anim.FlipAnimationHorizontal();
		facingRight = false;
	}
}

void ImpBoss::Sense()
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
	float vectorX = GetCenter().x - m_target->GetCenter().x;
	float vectorY = GetCenter().y - m_target->GetCenter().y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	
	if (m_target->GetCenter().x < GetCenter().x && facingRight || m_target->GetCenter().x >= GetCenter().x && !facingRight)
	{
		anim.FlipAnimationHorizontal();
		facingRight = !facingRight;
	}
	
}

void ImpBoss::Decide()
{
	if (dec == IMP_DECISION::ATTACK)
	{
		return;
	}

	if (attackCooldown > 0.f)
	{
		dec = IMP_DECISION::IDLE;
		anim.SetAnimation(texture, 1, false);
	}
	else if (distanceBetweenTarget > attackRange)
	{
		dec = IMP_DECISION::APPROACH;
		anim.SetAnimation(walkAtlas, 8, true);
	}
	else if (distanceBetweenTarget <= attackRange)
	{
		dec = IMP_DECISION::ATTACK;
		anim.SetAnimation(texture, 1, false);
		attackZonePos = m_target->GetCenter();
	}
}

void ImpBoss::Act(float deltaTime)
{
	attackCooldown -= deltaTime;
	switch (dec)
	{
	case IMP_DECISION::APPROACH:
		Approach(deltaTime);
		break;
	case IMP_DECISION::ATTACK:
		Attack(deltaTime);
		break;
	}
}

void ImpBoss::Render()
{
	/*
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	if (!facingRight)
	{
		src = { 0.f, 0.f, static_cast<float>(-texture.width), static_cast<float>(texture.height) };
	}
	*/
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	float rotation = 0.0f;

	anim.DrawAnimationPro(dst, origin, rotation, WHITE);




	Color color = WHITE;
	switch (dec)
	{
	case IMP_DECISION::APPROACH:
		color = YELLOW;
		break;
	case IMP_DECISION::ATTACK:
		color = RED;
		break;
	}
	color.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

	if (dec == IMP_DECISION::ATTACK)
	{
		DrawCircle(static_cast<int>(attackZonePos.x), static_cast<int>(attackZonePos.y), attackRange, color);
	}
}

void ImpBoss::NextPhase()
{
	baseSpeed *= 1.5f;
	chargeTime = 1.f;
	chargeCounter = chargeTime;

	PushPlayer();
	firstPhase = false;
}

void ImpBoss::PushPlayer()
{

	Vector2 force = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	float pushPower = 15.f;
	force.x *= -pushPower;
	force.y *= -pushPower;

	m_target->PushEntity(force);

}

void ImpBoss::Attack(float deltaTime)
{
	chargeCounter -= deltaTime;
	if (chargeCounter <= 0.f)
	{
		dec = IMP_DECISION::IDLE;
		SlamCollisionCheck();
		chargeCounter = chargeTime;
		attackCooldown = 0.1f;
	}
}

void ImpBoss::Approach(float deltaTime)
{
	Vector2 dir = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	x += -dir.x * deltaTime * baseSpeed;
	y += -dir.y * deltaTime * baseSpeed;
}

void ImpBoss::SlamCollisionCheck()
{
	if (m_target->AABBvsCircle(attackZonePos, attackRange))
	{
		PushPlayer();
		m_target->TakeDamage(20);
	}
}

void ImpBoss::DragCollisionCheck()
{
}
