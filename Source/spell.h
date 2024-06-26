#pragma once
#include <cmath>
#include "dynamic_entity.h"
#include <iostream>
#include <vector>
#include "button.h"
#include "config.h"
#include "zombie.h"
#include <algorithm>
#include "corpse.h"

class Effect  // Ice, Fire,  Poison
{
private:
	Color color = WHITE;
public:
	void ApplyEffect()
	{

	}
};

class Spell
{
private:
	
public:

	Camera2D* _cam{};
	DynamicEntity* m_caster = nullptr;
	float cooldownMax = 0.f;
	float cooldown = 0.f;
	float dmg = 0.f;
	float speed = 0.f;

	void ReduceCooldown(float percAmount);
	void IncreaseSpeed(float percAmount);
	void IncreaseDamage(float percAmount);
	void IncreaseProjectile(int amount) { amount; };

	void SetCamera(Camera2D &cam) {

		_cam = &cam; 
	}
	Camera2D* GetCamera() { return _cam; }
	virtual void Activate(DynamicEntity &caster){m_caster = &caster;}
	virtual void Update(float deltaTime) {deltaTime++;}
	virtual void Render(){}
	Vector2 GetNormalizedVector(float srcX, float srcY, float dstX, float dstY);
	float GetAngleFromVector(Vector2 vec);
	virtual void CollisionCheck(DynamicEntity* entity) { entity; }
};

class Bullet : public DynamicEntity
{
private:
	Config config;
	float timeAlive = 1.f;
	float size = 3.f;
	bool isAlive = true;
	int speed = 400;
	Texture2D txr{};
	float rotation = 0.f;
public:
	void SetTexture(Texture2D texture);
	void Update(float deltaTime) override;
	void Render() override;
	bool IsAlive(){return isAlive;}
	void SetRotation(float newRot) { rotation = newRot; }
};

class RangedBasicAttack : public Spell
{
private:
	int range = 10;

	const static int MAX_BULLETS = 5;
	int activeBullets = 0;
	Bullet* bulletList[MAX_BULLETS] = { nullptr };
	float velocityInfluence = 0.2f;
	Texture2D bulletTxr{};
	Config config;
public:
	RangedBasicAttack()
	{
		cooldownMax = 0.3f;
		dmg = 10.f;
	}
	void SetTexture(Texture2D texture){ bulletTxr = texture; }
	void Activate(DynamicEntity& caster) override;
	void Activate(DynamicEntity& caster, float dstX, float dstY);

	void Trigger(float srcX, float srcY, float dstX, float dstY);
	void Update(float deltaTime) override;
	void Render() override;
	void CollisionCheck(DynamicEntity* entity) override;
	~RangedBasicAttack();
};

class MeleeBasicAttack : public Spell{
private:

	float cooldown = 0.f;

	Vector2 swordPos{ 0.f, 0.f };
	float swordRadius = 0.f;


	float rotation = 0.f;

	float size = 5.f;
	float range = 100.f;
	float angle = 0.f;
	float limit = 45.f;
	bool activated = false;
	float width = 0.f;
	float height = 0.f;
	float vectorX = 0.f;
	float vectorY = 0.f;
	const float angleOffset = PI / 3.f;
	bool reverse = false;
	Texture2D swordTxr{};
	Config config;
public:
	MeleeBasicAttack() { cooldownMax = 0.2f; speed = 15.f; dmg = 1.f; }
	void SetTexture(Texture2D texture);
	void SetInitialSwing(bool reverseSwing){reverse = reverseSwing;}
	void Activate(DynamicEntity& caster) override;
	void Update(float deltaTime) override;
	void Render() override;
	void CollisionCheck(DynamicEntity* entity) override;
};

class ArcanistSignature : public Spell
{
private:
	Config config;

	static const int ballCount = 3;
	Vector2 balls[ballCount]{};
	
	float ballSize = 5.f;

	float originX = 0.f;
	float originY = 0.f;

	float activated = 0.f;
	float duration = 5.f;

	float minRange = 200.f;
	float maxRange = 400.f;
	float rangeRadius = 200.f;
	float angle = 0.f;
	const float offset = ((PI / 3.f) * 2.f);
	float speed = 2.f;
	float minSpeed = 2.f;
	float maxSpeed = 4.f;
	Texture2D texture = {};

	float ballWidth = 0.f;
	float ballHeight = 0.f;
	float ballRadius = 0.f;

public:
	ArcanistSignature(DynamicEntity& caster);
	virtual void Activate(DynamicEntity& caster);

	void Update(float deltaTime) override;
	void Render() override;
	void CollisionCheck(DynamicEntity* entity) override;
	bool BallCollisionCheck(DynamicEntity* entity, Vector2 ballPos);
};


class Soldier : public DynamicEntity
{
private:
	float lifeTime = 5.f;
	bool alive = true;
	float size = 15.f;
	float dmg = 0.f;
	float swordSize = 5.f;
	Vector2 swordPos{0.f, 0.f};
	float swordRadius = 0.f;
	float swordSpeed = 5.f;
	float range = 100.f;
	float rotation = 0.f;

	float srcX = 0.f;
	float srcY = 0.f;
	float dstX = 0.f;
	float dstY = 0.f;
	bool activated = false;
	float progress = 0.f;
	bool reverse = false;
	Texture2D statueTxr = {};
	Texture2D swordTxr = {};
	Config config;
	Vector2 swordCenter{ 0.f, 0.f };
public:
	Soldier(Texture2D newStatue, Texture2D newSword, float newDmg);
	void Update(float deltaTime);
	void Attack(Vector2 mousePos);
	void Render();
	bool IsAlive(){return alive;}
	void CollisionCheck(DynamicEntity* entity);
};

class SummonerSignature : public Spell
{
private:
	std::vector<Soldier*> soldiers;
	Texture2D soldierTxr = {};
	Texture2D swordTxr = {};
public:
	SummonerSignature();
	void Activate(DynamicEntity& caster) override;
	void Update(float deltaTime) override;
	void Render() override;
	void CollisionCheck(DynamicEntity* entity) override;
	~SummonerSignature();
};


class NecromancerSignature : public Spell
{
private:
	float range = 100.f;
	std::vector<Zombie> zombieList{}; 
	std::vector<EnemyCorpse>* deadEnemiesList{};
	std::vector<DynamicEntity>* enemiesList{};
public:
	void Activate(DynamicEntity& caster) override
	{
		m_caster = &caster;
		for (int i = 0; i < deadEnemiesList->size(); i++)
		{

			if (GetDistance(m_caster->GetCenter(), deadEnemiesList->at(i).position) <= range)
			{
				CreateZombie(deadEnemiesList->at(i));
				deadEnemiesList->erase(deadEnemiesList->begin() + i);
			}
		}
	}
	void CreateZombie(EnemyCorpse corpse)
	{
		Zombie newZombie;
		newZombie.Setup(corpse.txr, corpse.position, m_caster);
		zombieList.push_back(newZombie);
	}
	void SetCorpseList(std::vector<EnemyCorpse> &corpseList)
	{
		deadEnemiesList = &corpseList;
	}
	float GetDistance(Vector2 vec1, Vector2 vec2)
	{
		float vectorX = vec1.x - vec2.x;
		float vectorY = vec1.y - vec2.y;
		return static_cast<float>(sqrt((vectorX * vectorX) + (vectorY * vectorY)));
	}
	void Update(float deltaTime)  override
	{
		for (int i = 0; i < zombieList.size(); i++)
		{
			zombieList.at(i).Update(deltaTime);
		}
	}
	void Render()  override
	{
		
		for (int i = 0; i < zombieList.size(); i++)
		{
			zombieList.at(i).Render();
		}
		
	}
};


// Enchanter:
// Active: While Holding Spacebar, show a 3 way wheel.. 
// hovering over one and releasing spacebar will apply that effect and color to your weapons

// Signature Effects: Ice(Light Blue)(Slow), Fire(Orange)(Flat Bonus Dmg), Poison(Light Green)(Percentage Bonus Dmg)
// Mastered Effects: Dark Ice(Dark Blue/Purple)(Stun after every third hit), Blue Flame(Strong Blue), Venom
class EnchanterSignature : public Spell
{
private:
	Button buttons[3];
public:
	EnchanterSignature()
	{
		//buttons[0].Setup()
	}
	void Activate(DynamicEntity& caster) override
	{
		m_caster = &caster;
	}

	void Update(float deltaTime)  override
	{
		deltaTime++;
	}
	void Render()  override
	{
		if (!IsKeyDown(KEY_SPACE))
		{
			return;
		}
		buttons[0].Render();
		buttons[1].Render();
		buttons[2].Render();
	}
};


// Rogue:
// Active: Dash (invincible during)
// Unfinished Features: No invincibility, Optimize variables
class RogueSignature : public Spell
{
private:
	bool isDashing = false;
	float distance = 100.f;
	float dstX = 0.f;
	float dstY = 0.f;
	float srcX = 0.f;
	float srcY = 0.f;
	float dashSpeed = 10.f;
	float cooldown = 0.f;
	const float MAX_COOLDOWN = 2.f;
	float progress = 0.f;
public:
	void Activate(DynamicEntity& caster) override;
	void Update(float deltaTime) override;
	void Render() override;
};


// Paladin:
// Active: Heal Every Allied Unit Around Player
class PaladinSignature : public Spell
{
private:
	int healAmount = 25;
public:
	PaladinSignature()
	{
		cooldown = 0.f;
		cooldownMax = 5.f;
	}
	void Activate(DynamicEntity& caster) override
	{
		m_caster = &caster;
		if (cooldown <= 0.f)
		{
			m_caster->SetHealth(m_caster->GetHealth() + healAmount);
			cooldown = cooldownMax;
		}

	}

	void Update(float deltaTime)  override
	{
		cooldown -= deltaTime;
	
	}
	void Render()  override
	{
		
	}
};