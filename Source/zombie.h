#pragma once
#include "dynamic_entity.h"

class Zombie : public DynamicEntity
{
private:
	DynamicEntity* playerRef = nullptr;
	Texture2D txr{};
public:
	void Setup();
	void Sense();
	void Decide();
	void Act();
	void Update(float deltaTime) override;
	void Render() override;
};