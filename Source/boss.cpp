#include "boss.h"

Boss::Boss(DynamicEntity& target)
{
	m_target = &target;
	SetHealth(150);
}

void Boss::Sense()
{
	if (GetHealth() <= 50)
	{
		NextPhase();
	}
}

void Boss::Decide()
{
}

void Boss::Act(float deltaTime)
{
	deltaTime++;
}

void Boss::SpellCollisionCheck(DynamicEntity* other)
{
	other;
}

void Boss::Render()
{
	Enemy::Render();
	if (dec == BOSS_DECISION::ATTACK)
	{
		Color color = RED;
		color.a = 50;
		//DrawRectangle(x, y, width, height, color);
	}
}

void Boss::NextPhase()
{
}
