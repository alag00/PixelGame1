#pragma once
#include "dynamic_entity.h"
#include <iostream>
#include "spell.h"
#include "config.h"

enum class DECISION
{
	IDLE = 0,
	FLEE = 1,
	HUNT = 2,
	ATTACK = 3,
};

class Enemy : public DynamicEntity
{
private:
	float timer = 0.f;
	const float RESPOND_TIME = 0.25f;
	bool isActive = false;
	float invincibleTimer = 0.f;
	float invincibleTimeMax = 0.2f;
public:
	Config config;
	Texture2D texture = {};
	
	virtual void Sense(){}
	virtual void Decide(){}
	virtual void Act(float deltaTime) { deltaTime++; }
	void Update(float deltaTime) override;
	virtual void SpellCollisionCheck(DynamicEntity* other) { other; }
	bool IsAlive();
	virtual void Render() override;
	void SetTexture(Texture2D txr);
	Texture2D GetTexture() { return texture; }
	void InFrameCheck(Rectangle cam);
	bool GetIsActive() { return isActive; }
	void SetIsActive(bool newValue) { isActive = newValue; }
	void TakeDamage(int totalDmg) override;
	void InvincibleCheck(float deltaTime);
	virtual void SetRoomRef(Rectangle room) { room; }
};
class Imp : public Enemy
{
	float size = 10.f;

	DECISION decision = DECISION::IDLE;

	float shootTimer = 0.f;
	const float SHOOT_COOLDOWN = 0.3f;
	float distanceBetweenTarget = 0.f;
	const float ATTACK_RANGE = 200.f;
	const float DETECTION_RANGE = 600.f;
	float huntSpeed = 150.f;
	float fleeSpeed = 100.f;
	DynamicEntity* m_target = nullptr;
	RangedBasicAttack basic;
	Texture2D bulletTxr{};
public:
	Imp(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	void Render() override;
	void SpellCollisionCheck(DynamicEntity* other) override;
};

// increasing speed while hunting
class Crawler : public Enemy
{
private:
	DECISION decision = DECISION::IDLE;
	float minSpeed = 150.f;
	float maxSpeed = 10.f;
	float speed = minSpeed;
	DynamicEntity* m_target = nullptr;
	Config config;
public:
	Crawler(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	void Render() override;
};

// Slow but goes thru walls
class Specter : public Enemy
{
private:
	float speed = 100.f;
	DynamicEntity* m_target = nullptr;
	Config config;
public:
	Specter(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	void Render() override;
};

class Skeleton : public Enemy
{
private:
public:
};

// Slowly teleports towards player, deals dmg at end of teleport
class HellSpirit : public Enemy
{
private:
public:

};

