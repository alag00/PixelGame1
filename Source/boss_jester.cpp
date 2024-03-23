#include "boss_jester.h"

JesterBoss::JesterBoss(DynamicEntity& target)
{
	m_target = &target;
	SetHealth(100);
	SetMaxHealth(100);
	Texture2D tempTxr = LoadTexture("Resources/JesterBoss.png");
	SetTexture(tempTxr);
	walkAtlas = LoadTexture("Resources/JesterBossWalkAtlas.png");
	spinAtlas = LoadTexture("Resources/JesterBossSpinAtlas.png");
	jumpAtlas = LoadTexture("Resources/JesterBossJumpAtlas.png");
	anim.SetAnimation(texture, 1, false);

	width = static_cast<float>(texture.width * config.PIXEL_SCALE);
	height = static_cast<float>(texture.height * config.PIXEL_SCALE);

	if (m_target->GetCenter().x < GetCenter().x)
	{
		anim.FlipAnimationHorizontal();
		facingRight = false;
	}

	Texture2D jackTxr = LoadTexture("Resources/JackBoxAtlas.png");
	Texture2D bulletTxr = LoadTexture("Resources/FireBolt.png");
	for (int i = 0; i < jackCount; i++)
	{
		jackBoxes[i].SetTextures(jackTxr, bulletTxr);
	}

}

void JesterBoss::Sense()
{
	if (dec == JESTER_DECISION::TRANSFORMATION)
	{
		return;
	}
	if (firstPhase && GetHealth() <= 50)
	{
		NextPhase();
		return;
	}
	if (m_target == nullptr)
	{
		return;
	}

	

	float vectorX = GetCenter().x - m_target->GetCenter().x;
	float vectorY = GetCenter().y - m_target->GetCenter().y;
	distanceBetweenTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));


	if (dec == JESTER_DECISION::ATTACK || dec == JESTER_DECISION::JUMP)
	{
		return;
	}
	if (m_target->GetCenter().x < GetCenter().x && facingRight || m_target->GetCenter().x >= GetCenter().x && !facingRight)
	{
		anim.FlipAnimationHorizontal();
		facingRight = !facingRight;
	}

}

void JesterBoss::Decide()
{
	if (dec == JESTER_DECISION::ATTACK || dec == JESTER_DECISION::TRANSFORMATION || dec == JESTER_DECISION::JUMP)
	{
		return;
	}

	if (attackCooldown > 0.f)
	{
		dec = JESTER_DECISION::IDLE;
		anim.SetAnimation(texture, 1, false);
	}
	else if (distanceBetweenTarget > attackRange)
	{
		dec = JESTER_DECISION::APPROACH;
		anim.SetAnimation(walkAtlas, 8, true);
	}
	else if (distanceBetweenTarget <= attackRange)
	{
		dec = JESTER_DECISION::JUMP;
		
		SetDashPos();
	}
}

void JesterBoss::Act(float deltaTime)
{
	attackCooldown -= deltaTime;
	switch (dec)
	{
	case JESTER_DECISION::APPROACH:
		Approach(deltaTime);
		break;
	case JESTER_DECISION::JUMP:
		Jump(deltaTime);
		break;
	case JESTER_DECISION::ATTACK:
		Attack(deltaTime);
		break;
	case JESTER_DECISION::TRANSFORMATION:
		Transformation(deltaTime);
		break;
	}
	if (!firstPhase)
	{
		for (int i = 0; i < jackCount; i++)
		{
			jackBoxes[i].Update(deltaTime);
		}
	}
}

void JesterBoss::SpellCollisionCheck(DynamicEntity* other)
{
	if (dec == JESTER_DECISION::ATTACK )
	{
		Rectangle bossSize = { x,y,width,height };
		if (other->AABBvsAABBCheck(bossSize))
		{
			other->TakeDamage(10.f);
			PushPlayer();
		}
	}
	if (firstPhase)
	{
		return;
	}
	for (int i = 0; i < jackCount; i++)
	{
		jackBoxes[i].CollisionCheck(other);
	}
}

void JesterBoss::Render()
{

	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	float rotation = 0.0f;


	Color color = WHITE;
	switch (dec)
	{
	case JESTER_DECISION::APPROACH:
		color = YELLOW;
		break;
	case JESTER_DECISION::JUMP:
		color = RED;
		DrawCircle(static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), 2.f, color);
		DrawLine(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), color);
		break;
	case JESTER_DECISION::ATTACK:
		color = RED;
		dst.width = height;
		dst.height = width;
		DrawCircle(static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), 2.f, color);
		DrawLine(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), color);
		break;
	}
	/*
	if (dec == JESTER_DECISION::ATTACK || dec == JESTER_DECISION::JUMP)
	{
		dst.width = height;
		dst.height = width;
		DrawCircle(static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), 2.f, RED);
		DrawLine(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), static_cast<int>(dashTargetPos.x), static_cast<int>(dashTargetPos.y), RED);
	}
	*/
	anim.DrawAnimationPro(dst, origin, rotation, WHITE);




	
	color.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

	if (!firstPhase)
	{
		for (int i = 0; i < jackCount; i++)
		{
			jackBoxes[i].Render();
		}
	}
}

void JesterBoss::NextPhase()
{
	baseSpeed *= 1.5f;
	//chargeTime *= 0.5f;
	//chargeCounter = chargeTime;
	dashSpeed *= 1.5f;
	PushPlayer();
	//PushPlayer();
	superDashesLeft = 5;
	dec = JESTER_DECISION::TRANSFORMATION;
	
}

void JesterBoss::PushPlayer()
{

	Vector2 force = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	float pushPower = 15.f;
	force.x *= -pushPower;
	force.y *= -pushPower;

	//Vector2 force2 = {-vectorX, -vectorY};
	m_target->PushEntity(force);

	dec = JESTER_DECISION::IDLE;
	chargeCounter = chargeTime;
	attackCooldown = 3.f;
}

void JesterBoss::Attack(float deltaTime)
{
	failSafeDashCounter -= deltaTime;
	if (failSafeDashCounter > 0.f)
	{
		if (!Dash(deltaTime))
		{
			return;
		}
	}
	if (superDashesLeft > 0)
	{
		superDashesLeft--;
		chargeCounter = chargeTime;
		dec = JESTER_DECISION::JUMP;
		SetDashPos();
		return;
	}
	attackActive = true;
	chargeCounter = chargeTime;
	attackCooldown = 1.f;
	dec = JESTER_DECISION::IDLE;
}

void JesterBoss::SetDashPos()
{
	Vector2 dir = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);
	float range = 300.f;
	dashTargetPos.x = GetCenter().x - (dir.x * range);
	dashTargetPos.y = GetCenter().y - (dir.y * range);
	/*
	bool redo = false;
	float halfWidth = width / 2.f;
	float halfHeight = height / 2.f;
	do {
		redo = true;
		dashTargetPos.x = GetCenter().x - (dir.x * range);
		dashTargetPos.y = GetCenter().y - (dir.y * range);

		if (dashTargetPos.x >= room.x + halfWidth &&
			dashTargetPos.x <= (room.x + room.width - halfWidth) &&
			dashTargetPos.y >= room.y + halfHeight &&
			dashTargetPos.y <= (room.y + room.height - halfHeight))
		{
			redo = false;
		}
		range -= 50.f;
	} while (redo);
	*/
	anim.SetAnimation(jumpAtlas, 6, false);
	if (m_target->GetCenter().x < GetCenter().x && facingRight || m_target->GetCenter().x >= GetCenter().x && !facingRight)
	{
		anim.FlipAnimationHorizontal();
		facingRight = !facingRight;
	}
}

bool JesterBoss::Dash(float deltaTime)
{
	// Move towards target point
	Vector2 dir = GetNormalizedVectorTowardsPos(dashTargetPos.x, dashTargetPos.y);
	x += -dir.x * deltaTime * (baseSpeed * dashSpeed);
	y += -dir.y * deltaTime * (baseSpeed * dashSpeed);


	float vectorX = GetCenter().x - dashTargetPos.x;
	float vectorY = GetCenter().y - dashTargetPos.y;
	float distFromTarget = sqrt((vectorX * vectorX) + (vectorY * vectorY));
	if (distFromTarget <= 10.f)
	{
		return true;
	}
	return false;
}

void JesterBoss::Approach(float deltaTime)
{
	Vector2 dir = GetNormalizedVectorTowardsPos(m_target->GetCenter().x, m_target->GetCenter().y);

	x += -dir.x * deltaTime * baseSpeed;
	y += -dir.y * deltaTime * baseSpeed;
}

void JesterBoss::SetRoomRef(Rectangle roomRef)
{
	room = roomRef;
	roomCenter = {room.x + (room.width / 2.f), room.y + (room.height / 2.f) };
}

void JesterBoss::Transformation(float deltaTime)
{

	float vectorX = GetCenter().x - roomCenter.x;
	float vectorY = GetCenter().y - roomCenter.y;
	float distance = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	if (distance <= 10.f)
	{
		
		attackCooldown = 0.f;
		// Set jackbox pos and activation
		for (int i = 0; i < jackCount; i++)
		{
			jackBoxes[i].SetPosition(GetCenter());
		}

		float margin = 0.f;
		Vector2 jackPos = {room.x + margin, room.y + margin };
		jackBoxes[0].SetIdealPosAndTarget(jackPos, m_target);
		jackBoxes[0].FlipTxr();

		jackPos = { (room.x + room.width) - margin, room.y + margin };
		jackBoxes[1].SetIdealPosAndTarget(jackPos, m_target);

		jackPos = { room.x + margin, (room.y + room.height) - margin };
		jackBoxes[2].SetIdealPosAndTarget(jackPos, m_target);
		jackBoxes[2].FlipTxr();

		jackPos = { (room.x + room.width) - margin, (room.y + room.height) - margin };
		jackBoxes[3].SetIdealPosAndTarget(jackPos, m_target);

		firstPhase = false;
		dec = JESTER_DECISION::IDLE;
		return;
	}

	Vector2 dir = GetNormalizedVectorTowardsPos(roomCenter.x, roomCenter.y);
	x += -dir.x * deltaTime * baseSpeed * 2.f;
	y += -dir.y * deltaTime * baseSpeed * 2.f;

	
}

void JesterBoss::Jump(float deltaTime)
{
	chargeCounter -= deltaTime;
	if (chargeCounter <= 0.f)
	{
		failSafeDashCounter = 2.f;
		anim.SetAnimation(spinAtlas, 2, true);
		dec = JESTER_DECISION::ATTACK;
	}
}
