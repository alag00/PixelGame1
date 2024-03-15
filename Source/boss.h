#pragma once
#include "dynamic_entity.h"
#include "enemy.h"
#include "config.h"
#include "animation.h"

enum BOSS_DECISION
{
	IDLE,
	APPROACH,
	ATTACK,
};

class Boss : public Enemy
{
private:
	DynamicEntity* m_target = nullptr;
	BOSS_DECISION dec = IDLE;
	bool active = false;
	float baseSpeed = 150.f;

	float distanceBetweenTarget = 0.f;
	float attackRange = 100.f;
	float attackCooldown = 0.f;
	float chargeTime = 2.f;
	float chargeCounter = chargeTime;
	bool firstPhase = true;
	bool attackActive = false;

	Animator anim;
	Texture2D walkAtlas{};
public:
	Boss(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	//void Update(float deltaTime) override;
	void SpellCollisionCheck(DynamicEntity* other) override;
	void Render() override;
	void NextPhase();
	void PushPlayer();
	void Attack(float deltaTime);
	void Approach(float deltaTime);
};