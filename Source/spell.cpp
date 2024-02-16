#include "spell.h"

void Spell::ReduceCooldown(float amount)
{
	cooldownMax -= amount;
	if (cooldown <= 0.f)
	{
		cooldownMax = 0.1f;
	}
}

Vector2 Spell::GetNormalizedVector(float srcX, float srcY, float dstX, float dstY)
{
	float vectorX = srcX - dstX;
	float vectorY = srcY - dstY;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = (vectorX / length);
	vectorY = (vectorY / length);

	return Vector2{vectorX, vectorY};
}

float Spell::GetAngleFromVectors(Vector2 vec1, Vector2 vec2)
{
	float dot =  (vec1.x * vec2.x) + (vec1.y * vec2.y);
	float mag1 = sqrt((vec1.x * vec1.x) + (vec1.y * vec1.y));
	float mag2 = sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y));
	float radianAngle = acos(dot / (mag1 * mag2));

	return radianAngle * (180 / PI);
}



void Bullet::Update(float deltaTime)
{

	timeAlive -= deltaTime;
	x += velX * deltaTime * speed;
	y += velY * deltaTime * speed;
	if (timeAlive <= 0.f)
	{
		isAlive = false;
	}

}

void Bullet::Render()
{
	if (txr.id == 0)
	{
		DrawCircle(static_cast<int>(x), static_cast<int>(y), size, ORANGE);
		return;
	}
	Rectangle src = { 0.f, 0.f, static_cast<float>(txr.width), static_cast<float>(txr.height) };
	Rectangle dst = { x, y, static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	//Vector2 origin = { 0.f, 0.f };
	Vector2 origin = { static_cast<float>((txr.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((txr.width * config.PIXEL_SCALE) / 2.0f) };

	DrawTexturePro(txr, src, dst, origin, rotation, WHITE);
}


void RangedBasicAttack::Activate(DynamicEntity& caster)
{
	m_caster = &caster;
	if (GetCamera() == nullptr)
	{
		return;
	}
	//GetWorldToScreen2D
	//GetScreenToWorld2D
	float dstX = GetScreenToWorld2D(GetMousePosition(), *_cam).x;
	float dstY = GetScreenToWorld2D(GetMousePosition(), *_cam).y;
	Trigger(m_caster->x, m_caster->y, dstX, dstY);
}
void RangedBasicAttack::Activate(DynamicEntity& caster, float dstX, float dstY)
{
	m_caster = &caster;
	Trigger(m_caster->x, m_caster->y, dstX, dstY);
}
void RangedBasicAttack::Trigger(float srcX, float srcY, float dstX, float dstY) {

	// Deny condition
	if (activeBullets == MAX_BULLETS || cooldown > 0.f || m_caster == nullptr)
	{
		return;
	}
	// Search for Available space in Array
	int availableIndex = -1;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletList[i] == nullptr)
		{
			availableIndex = i;
			break;
		}
	}
	if (availableIndex == -1)
	{
		return;
	}
	cooldown = cooldownMax;

	// Create Bullet
	Bullet* newBullet = new Bullet();
	newBullet->SetTexture(bulletTxr);
	newBullet->x = srcX;
	newBullet->y = srcY;
	// Calculate bullet rotation
	

	// Bullet Direction
	/*
	float vectorX = srcX - dstX;
	float vectorY = srcY - dstY;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = (vectorX / length);
	vectorY = (vectorY / length);
	*/

	Vector2 dir = GetNormalizedVector(srcX, srcY, dstX, dstY);
	newBullet->velX = -dir.x + (m_caster->velX * velocityInfluence);
	newBullet->velY = -dir.y + (m_caster->velY * velocityInfluence);

	Vector2 startVec{0, -1};
	newBullet->SetRotation(GetAngleFromVectors(startVec, dir));
	bulletList[availableIndex] = newBullet;
	activeBullets++;
}
void RangedBasicAttack::Update(float deltaTime)
{
	cooldown -= deltaTime;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletList[i] == nullptr)
		{
			continue;
		}
		bulletList[i]->Update(deltaTime);
		if (!bulletList[i]->IsAlive())
		{
			delete bulletList[i];
			bulletList[i] = nullptr;
			activeBullets--;
		}
	}
}
void RangedBasicAttack::Render()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletList[i] == nullptr)
		{
			continue;
		}
		bulletList[i]->Render();
	}
}
RangedBasicAttack::~RangedBasicAttack()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		delete bulletList[i];
		bulletList[i] = nullptr;
	}
}


void MeleeBasicAttack::Activate(DynamicEntity& caster)
{
	// Deny condition
	if (activated || cooldown > 0.f)
	{
		return;
	}
	m_caster = &caster;
	activated = true;

	// Bullet Direction
	vectorX = m_caster->x - GetScreenToWorld2D(GetMousePosition(), *_cam).x;
	vectorY = m_caster->y - GetScreenToWorld2D(GetMousePosition(), *_cam).y;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	// Unit vector
	vectorX = -(vectorX / length);
	vectorY = -(vectorY / length);

	angle = atan2(vectorY, vectorX);

	//angle = acos(vectorX / sqrt((vectorX * vectorX) + (vectorY * vectorY)));
	if (!reverse)
	{
		limit = angle + angleOffset;
		angle -= angleOffset;
	}
	else
	{
		limit = angle - angleOffset;
		angle += angleOffset;
	}




}
void MeleeBasicAttack::Update(float deltaTime) 
{
	cooldown -= deltaTime;
	if (!activated)
	{
		return;
	}

	if (!reverse)
	{
		angle += deltaTime * speed;
	}
	else {
		angle -= deltaTime * speed;
	}

	xPos = m_caster->x + std::cos(angle) * range;
	yPos = m_caster->y + std::sin(angle) * range;



	if (!reverse && angle >= limit)
	{
		activated = false;
		angle = 0.f;
		cooldown = cooldownMax;
		reverse = true;
	}
	else if (reverse && angle <= limit)
	{
		activated = false;
		angle = 0.f;
		cooldown = cooldownMax;
		reverse = false;
	}

}
void MeleeBasicAttack::Render()
{
	if (activated)
	{
		DrawCircle(static_cast<int>(xPos), static_cast<int>(yPos), size, RED);
	}
}




ArcanistSignature::ArcanistSignature(DynamicEntity& caster)
{
	m_caster = &caster;
	rangeRadius = minRange;
	cooldownMax = 10.f;
	texture = LoadTexture("Resources/ArcanistBall.png");
	SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}
void ArcanistSignature::Activate(DynamicEntity& caster)
{
	if (cooldown > 0.f)
	{
		return;
	}
	m_caster = &caster;
	rangeRadius = maxRange;
	activated = duration;
	cooldown = cooldownMax;
}
void ArcanistSignature::Update(float deltaTime)
{
	cooldown -= deltaTime;
	activated -= deltaTime;
	rangeRadius = minRange;
	speed = minSpeed;
	if (activated > 0.f)
	{
		rangeRadius = maxRange;
		speed = maxSpeed;
	}
	// rotate Balls around player
	originX = m_caster->x;
	originY = m_caster->y;

	angle += deltaTime * speed;

	b1x = originX + std::cos(angle) * rangeRadius;
	b1y = originY + std::sin(angle) * rangeRadius;
	b2x = originX + std::cos(angle + offset) * rangeRadius;
	b2y = originY + std::sin(angle + offset) * rangeRadius;
	b3x = originX + std::cos(angle - offset) * rangeRadius;
	b3y = originY + std::sin(angle - offset) * rangeRadius;


}
void ArcanistSignature::Render() 
{
	float rotation = 0.0;
	Rectangle src = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst1 = { b1x, b1y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Rectangle dst2 = { b2x, b2y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Rectangle dst3 = { b3x, b3y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(texture.width / 2.0f), static_cast<float>(texture.height / 2.0f) };

	DrawTexturePro(texture, src, dst1, origin, rotation, WHITE);
	DrawTexturePro(texture, src, dst2, origin, rotation, WHITE);
	DrawTexturePro(texture, src, dst3, origin, rotation, WHITE);


	//DrawTexture(texture, static_cast<int>(b1x), static_cast<int>(b1y), RED);
	//DrawTexture(texture, static_cast<int>(b2x), static_cast<int>(b2y), GREEN);
	//DrawTexture(texture, static_cast<int>(b3x), static_cast<int>(b3y), BLUE);

}




Soldier::Soldier(Texture2D newStatue, Texture2D newSword)
{
	statueTxr = newStatue;
	swordTxr = newSword;
}

void Soldier::Update(float deltaTime)
{
	lifeTime -= deltaTime;
	if (lifeTime <= 0.f)
	{
		alive = false;
		return;
	}
	if (!activated)
	{
		return;
	}
	if (progress >= 1.f)
	{
		reverse = true;
	}
	if (!reverse)
	{
		progress += deltaTime * swordSpeed;
	}
	else
	{
		progress -= deltaTime * swordSpeed;
	}
	if (reverse && progress <= 0.f)
	{
		activated = false;
		return;
	}
	swordPosX = std::lerp(srcX, dstX, progress);
	swordPosY = std::lerp(srcY, dstY, progress);


}
void Soldier::Attack(Vector2 mousePos)
{
	if (progress > 0.f)
	{
		return;
	}
	// Calculate direction
	float vectorX = x - mousePos.x;
	float vectorY = y - mousePos.y;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = -(vectorX / length);
	vectorY = -(vectorY / length);

	dstX = x + (vectorX * range);
	dstY = y + (vectorY * range);

	srcX = x;
	srcY = y;

	progress = 0.f;
	reverse = false;
	activated = true;
}
void Soldier::Render()
{
	Rectangle src = { 0.f, 0.f, static_cast<float>(statueTxr.width), static_cast<float>(statueTxr.height) };
	Rectangle dst = { x, y, static_cast<float>(statueTxr.width * config.PIXEL_SCALE), static_cast<float>(statueTxr.height * config.PIXEL_SCALE) };
	Vector2 origin = { static_cast<float>(statueTxr.width / 2.0f), static_cast<float>(statueTxr.height / 2.0f) };
	float rotation = 0.0f;

	DrawTexturePro(statueTxr, src, dst, origin, rotation, WHITE);

	if (!activated)
	{
		return;
	}
	 src = { 0.f, 0.f, static_cast<float>(swordTxr.width), static_cast<float>(swordTxr.height) };
	 dst = { x, y, static_cast<float>(swordTxr.width * config.PIXEL_SCALE), static_cast<float>(swordTxr.height * config.PIXEL_SCALE) };
	 origin = { static_cast<float>(swordTxr.width / 2.0f), static_cast<float>(swordTxr.height / 2.0f) };
	

	DrawTexturePro(swordTxr, src, dst, origin, rotation, WHITE);
	//	DrawCircle(static_cast<int>(swordPosX), static_cast<int>(swordPosY), swordSize, RED);
}


SummonerSignature::SummonerSignature()
{
	soldierTxr = LoadTexture("Resources/SummonerStatue.png");
	swordTxr = LoadTexture("Resources/Sword.png");
	cooldownMax = 2.5f;
}
void SummonerSignature::Activate(DynamicEntity& caster) 
{
	if (cooldown > 0.f)
	{
		return;
	}
	m_caster = &caster;
	// summon soldier at mouse pos
	Soldier* newSoldier = new Soldier(soldierTxr, swordTxr);
	newSoldier->x = GetScreenToWorld2D(GetMousePosition(), *_cam).x;
	newSoldier->y = GetScreenToWorld2D(GetMousePosition(), *_cam).y;
	soldiers.push_back(newSoldier);
	cooldown = cooldownMax;
}
void SummonerSignature::Update(float deltaTime) 
{
	cooldown -= deltaTime;
	// when left clicking // Attack with all soldiers towards mouse pos
	
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *_cam);
		for (int i = 0; i < soldiers.size(); i++)
		{
			if (soldiers.at(i) == nullptr)
			{
				continue;
			}
			soldiers.at(i)->Attack(mousePos);
		}
	}
	for (int i = 0; i < soldiers.size(); i++)
	{
		if (soldiers.at(i) == nullptr)
		{
			continue;
		}
		soldiers.at(i)->Update(deltaTime);
		if (!soldiers.at(i)->IsAlive())
		{
			delete soldiers.at(i);
			soldiers.at(i) = nullptr;
		}
	}
}
void SummonerSignature::Render() 
{
	if (soldiers.size() <= 0)
	{
		return;
	}

	for (int i = 0; i < soldiers.size(); i++)
	{
		if (soldiers.at(i) == nullptr)
		{
			continue;
		}
		soldiers.at(i)->Render();
	}
}
SummonerSignature::~SummonerSignature()
{
	for (int i = 0; i < soldiers.size(); i++)
	{
		delete soldiers.at(i);
		soldiers.at(i) = nullptr;
	}
}



void RogueSignature::Activate(DynamicEntity& caster) 
{
	m_caster = &caster;
	if (cooldown > 0.f || m_caster->velX == 0.f && m_caster->velY == 0.f)
	{
		return;
	}
	// Calculate Dst Pos
	dstX = m_caster->x + m_caster->velX * distance;
	dstY = m_caster->y + m_caster->velY * distance;
	srcX = m_caster->x;
	srcY = m_caster->y;
	isDashing = true;
}
void RogueSignature::Update(float deltaTime) 
{
	cooldown -= deltaTime;
	if (!isDashing)
	{
		return;
	}
	progress += deltaTime * dashSpeed;
	m_caster->x = std::lerp(srcX, dstX, progress);
	m_caster->y = std::lerp(srcY, dstY, progress);
	if (progress >= 1.f)
	{
		isDashing = false;
		progress = 0.f;
		cooldown = MAX_COOLDOWN;
	}
}
void RogueSignature::Render() 
{
	if (!isDashing || m_caster == nullptr)
	{
		return;
	}
	//DrawCircle(static_cast<int>(srcX), static_cast<int>(srcY), 3.f, RED);
	//DrawCircle(static_cast<int>(dstX), static_cast<int>(dstY), 3.f, ORANGE);
	// Maybe some Trail 
}