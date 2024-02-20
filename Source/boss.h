#pragma once
#include "dynamic_entity.h"
#include "enemy.h"
#include "config.h"

enum BOSS_DECISION
{
	IDLE,
	KNOCKBACK,
	APPROACH,
	ATTACK,
};

class Boss : public Enemy
{
private:
	DynamicEntity* m_target = nullptr;
	BOSS_DECISION dec = IDLE;
	bool active = false;
	float baseSpeed = 20.f;
	float bonusSpeed = 1.f;
public:
	Boss(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	//void Update(float deltaTime) override;
	void SpellCollisionCheck(DynamicEntity* other) override;
	void Render() override;
	void NextPhase();
};