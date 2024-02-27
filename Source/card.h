#pragma once
#include "spell.h"
#include <cstdlib>
class Card
{
private:
	float amount = 0;
	const char* text = "Card:\n%i %%";
public:
	void SetText(const char* newText) { text = newText; }
	const char* GetText() { return text; }
	void SetAmount(float newValue) { amount = newValue; }
	float GetAmount() { return amount; }
	virtual void TriggerEffect(Spell* spell) { spell; }
};

class SpeedCard : public Card
{
private:
public:
	SpeedCard() { SetText("Increase\nSpeed:\n%i %%"); }
	void TriggerEffect(Spell* spell) override
	{
		spell->IncreaseSpeed(GetAmount());
	}
};

class CooldownCard : public Card
{
private:
public:
	CooldownCard() { SetText("Reduce\nCooldown:\n%i %%"); }
	void TriggerEffect(Spell* spell) override
	{
		spell->ReduceCooldown(GetAmount());
	}
};

class DamageCard : public Card
{
private:
public:
	DamageCard() { SetText("Increase\nDamage:\n%i %%"); }
	void TriggerEffect(Spell* spell) override
	{
		spell->IncreaseDamage(GetAmount());
	}
};

class ProjectileCard : public Card
{
private:
public:
	ProjectileCard() { SetText("Increase\nProjectiles:\n%i %%"); }
	void TriggerEffect(Spell* spell) override
	{
		spell;
		//spell->IncreaseSpeed(GetAmount());
	}
};

class HealthCard : public Card
{
private:
public:
	HealthCard() { SetText("Increase\nHealth:\n%i %%"); }
	void TriggerEffect(Spell* spell) override
	{
		spell;
		//spell->IncreaseSpeed(GetAmount());
	}
};

