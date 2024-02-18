#pragma once
#include <cmath>
#include "dynamic_entity.h"
#include <iostream>
#include <vector>
#include "button.h"
#include "config.h"

class Effect  // Ice, Fire, 
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
	void ReduceCooldown(float amount);

	void SetCamera(Camera2D &cam) {

		_cam = &cam; 
	}
	Camera2D* GetCamera() { return _cam; }
	virtual void Activate(DynamicEntity &caster){m_caster = &caster;}
	virtual void Update(float deltaTime) {deltaTime++;}
	virtual void Render(){}
	Vector2 GetNormalizedVector(float srcX, float srcY, float dstX, float dstY);
	float GetAngleFromVectors(Vector2 vec1, Vector2 vec2);
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
	//const float MAX_COOLDOWN = 0.3f;
	float cooldown = 0.f;
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
	//const float MAX_COOLDOWN = 0.2f;
	float cooldown = 0.f;

	float xPos = 0.f;
	float yPos = 0.f;

	float size = 5.f;
	float range = 40.f;
	float angle = 0.f;
	float speed = 15.f;
	float limit = 45.f;
	bool activated = false;

	float vectorX = 0.f;
	float vectorY = 0.f;
	const float angleOffset = PI / 3.f;
	bool reverse = false;
public:
	MeleeBasicAttack(){cooldownMax = 0.2f;}

	void SetInitialSwing(bool reverseSwing){reverse = reverseSwing;}
	void Activate(DynamicEntity& caster) override;
	void Update(float deltaTime) override;
	void Render() override;
};

// Signature Spells
// Activates on Spacebar


// Arcanist:
// Passive: Balls rotating around player
// Active: Temporarly Expand range on balls 
class ArcanistSignature : public Spell
{
private:
	Config config;
	float b1x = 0.f;
	float b1y = 0.f;			 
	float b2x = 0.f; 
	float b2y = 0.f;
	float b3x = 0.f; 
	float b3y = 0.f;

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

public:
	ArcanistSignature(DynamicEntity& caster);
	virtual void Activate(DynamicEntity& caster);

	void Update(float deltaTime) override;
	void Render() override;
};



// Summoner:
// Active: Summon Static Solder that uses Thrust attacks towards mousePos when player LeftClicks
// Cooldown: 2.5f  // Soldier Duration: 5.f
class Soldier : public DynamicEntity
{
private:
	float lifeTime = 5.f;
	bool alive = true;
	float size = 15.f;

	float swordSize = 5.f;
	float swordPosX = 0.f;
	float swordPosY = 0.f;
	float swordSpeed = 5.f;
	float range = 100.f;
	//bool attacking = false;
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
public:
	Soldier(Texture2D newStatue, Texture2D newSword);
	void Update(float deltaTime);
	void Attack(Vector2 mousePos);
	void Render();
	bool IsAlive(){return alive;}
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
	~SummonerSignature();
};


// Necromancer:
// Passive: Revive killed enemies as allies
class NecromancerSignature : public Spell
{
private:
	std::vector<DynamicEntity> zombieList;
public:
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
// Active: Heal Every Unit Around Player
class PaladinSignature : public Spell
{
private:
public:
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
		
	}
};