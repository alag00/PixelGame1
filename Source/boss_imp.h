#pragma once


#include "dynamic_entity.h"
#include "enemy.h"
#include "config.h"
#include "animation.h"



/*
* Imp Boss AI Cycle:
* Move Towards Player
* Charge Attack
* Attack
*
* 2ND PHASE
* Do a Morde E into Charged attack
* 
*/


class ImpBoss : public Enemy
{
private:
	
	enum IMP_DECISION
	{
		IDLE,
		APPROACH,
		ATTACK,
	};
	IMP_DECISION dec = IMP_DECISION::IDLE;
	DynamicEntity* m_target = nullptr;
	bool active = false;
	float baseSpeed = 150.f;

	float distanceBetweenTarget = 0.f;
	float attackRange = 100.f;
	float attackCooldown = 0.f;
	float chargeTime = 1.f;
	float chargeCounter = chargeTime;
	bool firstPhase = true;
	bool attackActive = false;
	bool facingRight = true;
	Vector2 attackZonePos{ 0.f,0.f };

	Animator anim;
	Texture2D walkAtlas{};
public:
	ImpBoss(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	//void Update(float deltaTime) override;
	//void SpellCollisionCheck(DynamicEntity* other) override;
	void Render() override;
	void NextPhase();
	void PushPlayer();
	void Attack(float deltaTime);
	void Approach(float deltaTime);
	void SlamCollisionCheck();
	void DragCollisionCheck();
};