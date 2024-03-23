#pragma once
#include "dynamic_entity.h" 
#include "spell.h"
#include <vector>
#include "config.h"
#include "game_info.h"
#include "corpse.h"

class Player : public DynamicEntity
{
private:
	bool isPause = false;
	Camera2D* _cam{};
	Config config;
	Texture2D texture = {};
	//Texture2D backTxr{};
	const static int MAX_SPELL_SLOTS = 10;
	Spell* spellbook[MAX_SPELL_SLOTS] = {nullptr};

	float acceleration = 50.f;
	float startSpeed = 300.f;
	float speed = startSpeed;
	float maxSpeed = 600.f;
	bool isRunning = false;


	float offsetTextureX = 0.f;
	float offsetTextureY = 0.f;

	bool lookingRight = true;
	//bool lookingDown = true;
public:
	//int velX = 0;
	//int velY = 0;
	Player(GameInfo info, Camera2D& cam, std::vector<EnemyCorpse>& corpseList);
	~Player();
	//void SetStartPosition(Vector2 pos);
	void Control(float deltaTime);
	void ReduceVelocity(float deltaTime);
	void Movement(float deltaTime);
	void UpdateSpellBook(float deltaTime);
	void Update(float deltaTime) override;
	void OnCollision(Entity* other) override;
	void Render() override;
	void SpellCollisionCheck(DynamicEntity* other);
	void SetPlayerPauseMode(bool newValue) { isPause = newValue; };
	void UpdateTextureFacing();
	
	// Class setup
	void ClassSetupArcanist();
	void ClassSetupSummoner();
	void ClassSetupNecromancer(std::vector<EnemyCorpse>& corpseList);
	void ClassSetupEnchanter();
	void ClassSetupRogue();
	void ClassSetupPaladin();



	
};