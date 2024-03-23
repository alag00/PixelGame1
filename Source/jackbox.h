#pragma once
#include "dynamic_entity.h"
#include "spell.h"
#include "config.h"
#include "animation.h"

class JackBox : public DynamicEntity
{
private:
	Texture2D txr{};
	RangedBasicAttack basic;
	Config config;
	Animator anim;
	float shootTimer = 0.f;
	Vector2 idealPos{ 0.f,0.f };
	bool onPos = false;
	DynamicEntity* target = nullptr;
public:
	void SetTextures(Texture2D newTxr, Texture2D bulletTxr);
	void Update(float deltaTime) override;
	void Render() override;
	void CollisionCheck(DynamicEntity* entity);
	void SetIdealPosAndTarget(Vector2 pos, DynamicEntity* newTarget);
	void MoveToPos(float deltaTime);
	void FlipTxr();
};