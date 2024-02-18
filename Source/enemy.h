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
	//float size = 10.f;
	//Color color = RED;
	float timer = 0.f;
	const float RESPOND_TIME = 0.25f;
	Config config;
public:
	Texture2D texture = {};
	
	virtual void Sense(){}
	virtual void Decide(){}
	virtual void Act(float deltaTime) { deltaTime++; }
	void Update(float deltaTime) override;
	virtual void SpellCollisionCheck(DynamicEntity* other) { other; }
	bool IsAlive();
	virtual void Render() override;
};

// Normal Enemy, Range or Melee
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
	Config config;
	RangedBasicAttack basic;
public:
	Imp(DynamicEntity& target);
	void Sense() override;
	void Decide() override;
	void Act(float deltaTime) override;
	void Render() override;
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
//
class Skeleton : public Enemy
{
private:
public:
};

//
class HellSpirit : public Enemy
{
private:
public:

};

