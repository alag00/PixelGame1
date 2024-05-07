#pragma once
#include "card.h"
#include "player.h"
#include "game_info.h"
#include "button.h"
#include "config.h"
#include <cstdlib>
class CardManager
{
private:
	Player* playerRef = nullptr;
	GameInfo* gameInfoRef = nullptr;
	bool active = false;
	
	static const int cardAmount = 3;
	Button cardButtons[cardAmount];
	Card* cards[cardAmount] = { nullptr };
	Config config;
	Texture2D cardTxr{};
	float buffAmount = 0.f;
public:
	CardManager();
	~CardManager();
	void SetPlayerRef(Player* ref) { playerRef = ref; }
	void SetGameInfoRef(GameInfo& ref) { gameInfoRef = &ref; }
	void Update(Camera2D cam); 
	void Activate(); 
	void DeActivate();
	void Render(); 
};