#include "card_manager.h"

CardManager::CardManager()
{
	cardTxr = LoadTexture("Resources/Texture/DietyCard.png");

	int randNum = (rand() % 19 + 2) * 5; // 2 to 20
	buffAmount = static_cast<float>(randNum / 100.f);

	cards[0] = new SpeedCard();
	cards[1] = new CooldownCard();
	cards[2] = new DamageCard();

	for (int i = 0; i < cardAmount; i++)
	{
		cardButtons[i].width = (cardTxr.width * config.PIXEL_SCALE) ;
		cardButtons[i].height = (cardTxr.height * config.PIXEL_SCALE) ;
		cardButtons[i].SetupImage(cardTxr);
		cards[i]->SetAmount(buffAmount);
	}
	
}

CardManager::~CardManager()
{
	for (int i = 0; i < cardAmount; i++)
	{
		delete cards[i];
		cards[i] = nullptr;
	}
}

void CardManager::Update(Camera2D cam)
{
	if (!active)
	{
		return;
	}

	float startX = (cardButtons[0].width * 2.f) - (cardButtons[0].width / 2.f);
	float startY = (config.screenHeight / 2.f) - (cardButtons[0].height / 2.f);
	Vector2 vec = { startX, startY };
	for (int i = 0; i < cardAmount; i++)
	{
		cardButtons[i].x = GetScreenToWorld2D(vec, cam).x;
		cardButtons[i].y = GetScreenToWorld2D(vec, cam).y;
		vec.x += config.screenWidth / cardAmount;
	}

	Vector2 mousePos = GetMousePosition();
	for (int i = 0; i < cardAmount; i++)
	{
		if (cardButtons[i].Update(GetScreenToWorld2D(mousePos, cam)))
		{

			gameInfoRef->equippedCards.push_back(cards[i]);
			DeActivate();
		}
	}
}

void CardManager::Activate()
{
	active = true;
	playerRef->SetInvincibility(true);
	

	
}

void CardManager::DeActivate()
{
	active = false;
	playerRef->SetInvincibility(false);

}

void CardManager::Render()
{
	if (!active)
	{
		return;
	}
	for (int i = 0; i < cardAmount; i++)
	{
		cardButtons[i].Render();
		int convertedAmount = static_cast<int>(buffAmount * 100);
		const char* text = TextFormat(cards[i]->GetText(), convertedAmount);
		DrawText(text, static_cast<int>(cardButtons[i].x), static_cast<int>(cardButtons[i].y), 15, WHITE);
	}
}
