#include "jackbox.h"

void JackBox::SetTextures(Texture2D newTxr, Texture2D bulletTxr)
{
	txr = newTxr;
	width = (txr.width * config.PIXEL_SCALE) / 4.f;
	height = txr.height * config.PIXEL_SCALE;
	anim.SetAnimation(txr, 4, true);
	basic.SetTexture(bulletTxr);
}

void JackBox::Update(float deltaTime)
{
	if (!onPos)
	{
		MoveToPos(deltaTime);
		return;
	}
	shootTimer -= deltaTime;
	if (shootTimer <= 0.f)
	{
		basic.Activate(*this, target->GetCenter().x, target->GetCenter().y);
		shootTimer = 3.f;
	}
	basic.Update(deltaTime);
}

void JackBox::Render()
{
	Rectangle dst = { x, y, width, height };
	Vector2 origin = { 0.f, 0.f };
	float rotation = 0.0f;

	anim.DrawAnimationPro(dst, origin, rotation, WHITE);

	basic.Render();
}

void JackBox::CollisionCheck(DynamicEntity* entity)
{
	basic.CollisionCheck(entity);
}

void JackBox::SetIdealPosAndTarget(Vector2 pos, DynamicEntity* newTarget)
{
	idealPos = pos;
	target = newTarget;
}

void JackBox::MoveToPos(float deltaTime)
{
	Vector2 dir = GetNormalizedVectorTowardsPos(idealPos.x, idealPos.y);
	x += -dir.x * deltaTime * 200.f;
	y += -dir.y * deltaTime * 200.f;
	
	float vectorX = GetCenter().x - idealPos.x;
	float vectorY = GetCenter().y - idealPos.y;
	float distFromTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));
	if (distFromTarget <= 5.f)
	{
		onPos = true;
		anim.FlipAnimationHorizontal();
	}
}

void JackBox::FlipTxr()
{
	anim.FlipAnimationHorizontal();
}
