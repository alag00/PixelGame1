#pragma once
#include "dynamic_entity.h" 
#include "spell.h"
#include <vector>
#include "config.h"

class Player : public DynamicEntity
{
private:
	Camera2D* _cam{};
	Config config;
	Texture2D texture = {};
	const static int MAX_SPELL_SLOTS = 10;
	Spell* spellbook[MAX_SPELL_SLOTS] = {nullptr};

	float acceleration = 10.f;
	float startSpeed = 200.f;
	float speed = startSpeed;
	float maxSpeed = 600.f;
	bool isRunning = false;


	float offsetTextureX = 0.f;
	float offsetTextureY = 0.f;
public:
	//int velX = 0;
	//int velY = 0;
	Player(int role, Camera2D& cam);
	~Player();
	//void SetStartPosition(Vector2 pos);
	void Control();
	void Movement(float deltaTime);
	void UpdateSpellBook(float deltaTime);
	void Update(float deltaTime) override;
	void OnCollision(Entity* other) override;
	void Render() override;
	
	// Class setup
	void ClassSetupArcanist();
	void ClassSetupSummoner();
	void ClassSetupNecromancer();
	void ClassSetupEnchanter();
	void ClassSetupRogue();
	void ClassSetupPaladin();
	
};