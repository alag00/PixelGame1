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

	return (radianAngle * (180 / PI));
}



void Bullet::SetTexture(Texture2D texture)
{
	txr = texture;
	width = texture.width * config.PIXEL_SCALE;
	height = texture.height * config.PIXEL_SCALE;
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
	Rectangle dst = { x + ((txr.width * config.PIXEL_SCALE) / 2.f), y + ((txr.height * config.PIXEL_SCALE) / 2.f), static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	//Vector2 origin = { 0.f, 0.f };
	Vector2 origin = { static_cast<float>((txr.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((txr.width * config.PIXEL_SCALE) / 2.0f) };

	//rotation = 0.f;
	DrawTexturePro(txr, src, dst, origin, rotation, WHITE);

	Color color = YELLOW;
	color.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);
}


void RangedBasicAttack::Activate(DynamicEntity& caster)
{
	m_caster = &caster;
	if (GetCamera() == nullptr)
	{
		return;
	}
	float dstX = GetScreenToWorld2D(GetMousePosition(), *_cam).x;
	float dstY = GetScreenToWorld2D(GetMousePosition(), *_cam).y;
	Trigger(m_caster->GetCenter().x, m_caster->GetCenter().y, dstX, dstY);
}
void RangedBasicAttack::Activate(DynamicEntity& caster, float dstX, float dstY)
{
	m_caster = &caster;
	Trigger(m_caster->GetCenter().x, m_caster->GetCenter().y, dstX, dstY);
}
void RangedBasicAttack::Trigger(float srcX, float srcY, float dstX, float dstY) 
{
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
	// Offset positions for texture
	srcX -= (bulletTxr.width * config.PIXEL_SCALE) / 2.f;
	srcY -= (bulletTxr.height * config.PIXEL_SCALE) / 2.f;

	dstX -= (bulletTxr.width * config.PIXEL_SCALE) / 2.f;
	dstY -= (bulletTxr.height * config.PIXEL_SCALE) / 2.f;


	// Create Bullet
	Bullet* newBullet = new Bullet();
	newBullet->SetTexture(bulletTxr);
	newBullet->x = srcX;
	newBullet->y = srcY;

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

	Vector2 startVec{-1, 1};
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
void RangedBasicAttack::CollisionCheck(DynamicEntity* entity)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletList[i] == nullptr)
		{
			continue;
		}
		if (bulletList[i]->x  < entity->x + entity->width
			&& bulletList[i]->x + bulletList[i]->width  > entity->x
			&& bulletList[i]->y  < entity->y + entity->height
			&& bulletList[i]->y + bulletList[i]->height  > entity->y)
		{
			entity->TakeDamage(10);
			delete bulletList[i];
			bulletList[i] = nullptr;
			activeBullets--;
		}
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


void MeleeBasicAttack::SetTexture(Texture2D texture)
{
	swordTxr = texture;
	width = swordTxr.width * config.PIXEL_SCALE;
	height = swordTxr.height * config.PIXEL_SCALE;
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
	vectorX = m_caster->GetCenter().x - GetScreenToWorld2D(GetMousePosition(), *_cam).x;
	vectorY = m_caster->GetCenter().y - GetScreenToWorld2D(GetMousePosition(), *_cam).y;

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

	xPos = m_caster->GetCenter().x + std::cos(angle) * range;
	yPos = m_caster->GetCenter().y + std::sin(angle) * range;



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
	if (!activated)
	{
		return;
		//DrawCircle(static_cast<int>(xPos), static_cast<int>(yPos), size, RED);
	}
	if (swordTxr.id == 0)
	{
		DrawCircle(static_cast<int>(xPos), static_cast<int>(yPos), size, ORANGE);
		return;
	}
	
	//float x = xPos - (width / 2.f);
	//float y = yPos - (height / 2.f);
	// Nearest Point
	/*
	Vector2 nearestPoint;
	nearestPoint.x = std::max(x, std::min(x + width, m_caster->GetCenter().x));
	nearestPoint.y = std::max(y, std::min(y + height, m_caster->GetCenter().y));
	DrawCircle(static_cast<int>(nearestPoint.x), static_cast<int>(nearestPoint.y), 3.f, RED);
	*/
	Vector2 nP{ 0.f, 0.f };
	nP.x = m_caster->GetCenter().x - xPos;
	nP.y = m_caster->GetCenter().y - yPos;

	float magV = sqrt((nP.x * nP.x) + (nP.y * nP.y));
	float aX = xPos + (nP.x / magV) * (height / 2.f);
	float aY = yPos + (nP.y / magV) * (height / 2.f);

	// Texture
	Rectangle src = { 0.f, 0.f, static_cast<float>(swordTxr.width), static_cast<float>(swordTxr.height) };
	Rectangle dst = { aX , aY , width, height};
	Vector2 origin = { width / 2.0f, height };
	//Vector2 origin = { static_cast<float>((swordTxr.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((swordTxr.width * config.PIXEL_SCALE) / 2.0f) };
	rotation = (angle * (180 / PI)) + 90.f ;
	DrawTexturePro(swordTxr, src, dst, origin, rotation, WHITE);
	
	
	// Collision Circle
	Color color = YELLOW;
	color.a = 50;
	DrawCircle(static_cast<int>(xPos), static_cast<int>(yPos), height / 2.f, color);
	//DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);


}

void MeleeBasicAttack::CollisionCheck(DynamicEntity* entity)
{
	float x = xPos - (width / 2.f);
	float y = yPos - (height / 2.f);

	Vector2 nearestPoint;
	nearestPoint.x = std::max(entity->x, std::min(entity->x + entity->width, xPos));
	nearestPoint.y = std::max(entity->y, std::min(entity->y + entity->height, yPos));
	
	float vX = nearestPoint.x - x;
	float vY = nearestPoint.y - y;

	float distance = sqrt((vX * vX) + (vY * vY));
	if (distance <= height / 2.f)
	{
		entity->TakeDamage(10);
	}
	
}





ArcanistSignature::ArcanistSignature(DynamicEntity& caster)
{
	m_caster = &caster;
	rangeRadius = minRange;
	cooldownMax = 10.f;
	texture = LoadTexture("Resources/ArcanistBall.png");
	ballWidth = texture.width * config.PIXEL_SCALE;
	ballHeight = texture.height * config.PIXEL_SCALE;
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
	Rectangle dst1 = { b1x, b1y, ballWidth, ballHeight };
	Rectangle dst2 = { b2x, b2y, ballWidth, ballHeight };
	Rectangle dst3 = { b3x, b3y, ballWidth, ballHeight };
	//Vector2 origin = { static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((texture.height * config.PIXEL_SCALE) / 2.0f) };
	Vector2 origin = { 0.f, 0.f };

	DrawTexturePro(texture, src, dst1, origin, rotation, WHITE);
	DrawTexturePro(texture, src, dst2, origin, rotation, WHITE);
	DrawTexturePro(texture, src, dst3, origin, rotation, WHITE);

	Color color = YELLOW;
	color.a = 50;
	DrawRectangle(static_cast<int>(b1x), static_cast<int>(b1y), static_cast<int>(ballWidth), static_cast<int>(ballHeight), color);
	DrawRectangle(static_cast<int>(b2x), static_cast<int>(b2y), static_cast<int>(ballWidth), static_cast<int>(ballHeight), color);
	DrawRectangle(static_cast<int>(b3x), static_cast<int>(b3y), static_cast<int>(ballWidth), static_cast<int>(ballHeight), color);
	//DrawTexture(texture, static_cast<int>(b1x), static_cast<int>(b1y), RED);
	//DrawTexture(texture, static_cast<int>(b2x), static_cast<int>(b2y), GREEN);
	//DrawTexture(texture, static_cast<int>(b3x), static_cast<int>(b3y), BLUE);

}

void ArcanistSignature::CollisionCheck(DynamicEntity* entity)
{
	if (b1x  < entity->x + entity->width
		&& b1x + ballWidth  > entity->x
		&& b1y  < entity->y + entity->height
		&& b1y + ballHeight  > entity->y)
	{
		entity->TakeDamage(1);
	}
	if (b2x  < entity->x + entity->width
		&& b2x + ballWidth  > entity->x
		&& b2y  < entity->y + entity->height
		&& b2y + ballHeight  > entity->y)
	{
		entity->TakeDamage(1);
	}
	if (b3x  < entity->x + entity->width
		&& b3x + ballWidth  > entity->x
		&& b3y  < entity->y + entity->height
		&& b3y + ballHeight  > entity->y)
	{
		entity->TakeDamage(1);
	}
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