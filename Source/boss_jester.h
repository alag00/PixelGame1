#pragma once

#include "dynamic_entity.h"
#include "enemy.h"
#include "config.h"
#include "animation.h"
#include "jackbox.h"

class JesterBoss : public Enemy
{
private:
	
	enum JESTER_DECISION
	{
		IDLE,
		APPROACH,
		JUMP,
		ATTACK,
		TRANSFORMATION,
	};
	JESTER_DECISION dec = JESTER_DECISION::IDLE;
	DynamicEntity* m_target = nullptr;
	bool active = false;
	float baseSpeed = 150.f;

	float failSafeDashCounter = 0.f;
	float dashSpeed = 2.f;
	float distanceBetweenTarget = 0.f;
	float attackRange = 200.f;
	float attackCooldown = 0.f;
	float chargeTime = 1.0f;
	float chargeCounter = chargeTime;
	bool firstPhase = true;
	bool attackActive = false;
	bool facingRight = true;
	int superDashesLeft = 0;
	Vector2 dashTargetPos{ 0.f, 0.f };
	Animator anim;
	Texture2D walkAtlas{};
	Texture2D spinAtlas{};
	Texture2D jumpAtlas{};
	Vector2 roomCenter{ 0.f,0.f };
	Rectangle room{ 0.f,0.f,0.f,0.f };
	static const int jackCount = 4;
	JackBox jackBoxes[jackCount];
public:
	JesterBoss(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	void SpellCollisionCheck(DynamicEntity* other) override;
	void Render() override;
	void NextPhase();
	void PushPlayer();
	void Attack(float deltaTime);
	void SetDashPos();
	bool Dash(float deltaTime);
	void Approach(float deltaTime);
	void SetRoomRef(Rectangle roomRef) override;
	void Transformation(float deltaTime);
	void Jump(float deltaTime);
};