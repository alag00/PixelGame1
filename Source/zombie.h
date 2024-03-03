#pragma once
#include "dynamic_entity.h"
#include "config.h"
#include <vector>
#include <cmath>
enum class ZOMBIE_DECISION
{
	IDLE = 0,
	FOLLOW = 1,
	HUNT = 2,
	ATTACK = 3,
};

class Zombie : public DynamicEntity
{
private:
	ZOMBIE_DECISION dec = ZOMBIE_DECISION::IDLE;
	DynamicEntity* playerRef = nullptr;
	std::vector<DynamicEntity*>* enemyList{};
	DynamicEntity* target = nullptr;

	Texture2D txr{};
	Config config;
	float timer = 0.f;
	const float RESPOND_TIME = 0.25f;

	float distPtoE = 0.f;
	float distZtoE = 0.f;
	float distZtoP = 0.f;
	float attackRange = 100.f;
	float protectRange = 300.f;
	float huntRange = 400.f;
	float followRange = 500.f;
	float speed = 2.f;
public:
	void Setup(Texture2D newTxr, Vector2 newPos, DynamicEntity* player);
	void Sense();
	void Decide();
	void Act(float deltaTime);
	void Update(float deltaTime) override;
	void Render() override;
	float GetDistance(Vector2 vec1, Vector2 vec2);
	void MoveTowards(Vector2 pos);
};