#include "player.h"
#include <iostream>

Player::Player(GameInfo info, Camera2D& cam, std::vector<EnemyCorpse>& corpseList)
{
	_cam = &cam;
	switch (info.playerClass)
	{
	case 0:
		ClassSetupArcanist();
		break;
	case 1:
		ClassSetupArcanist();
		break;
	case 2:
		ClassSetupSummoner();
		break;
	case 3:
		ClassSetupNecromancer(corpseList);
		break;
	case 4:
		ClassSetupEnchanter();
		break;
	case 5:
		ClassSetupRogue();
		break;
	case 6:
		ClassSetupPaladin();
		break;
	}
	
	SetTextureFilter(texture, TEXTURE_FILTER_POINT);

	width = texture.width * config.PIXEL_SCALE;
	height = texture.height * config.PIXEL_SCALE;


	for (int i = 0; i < info.equippedCards.size(); i++)
	{
		for (int j = 0; j < MAX_SPELL_SLOTS; j++)
		{
			if (spellbook[j] == nullptr)
			{
				continue;
			}
			info.equippedCards.at(i)->TriggerEffect(spellbook[j]);
		}
	}

}

Player::~Player()
{
	
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		delete spellbook[i];
		spellbook[i] = nullptr;
	}
	
}


void Player::Update(float deltaTime)
{

	if (isPause)
	{
		return;
	}

	isColliding = false;
	ReduceVelocity(deltaTime);
	Control(deltaTime);
	Movement(deltaTime);
	UpdateSpellBook(deltaTime);
	UpdateTextureFacing();
}

void Player::OnCollision(Entity* other)
{
	
	isColliding = true;
	
	float pushForce = 3.5f;

	float vectorX = x - other->x;
	float vectorY = y - other->y;

	float length = sqrt((vectorX * vectorX) + (vectorY * vectorY));

	vectorX = (vectorX / length);
	vectorY = (vectorY / length);

	x += vectorX * pushForce;
	y += vectorY * pushForce;
		
}

void Player::Render()
{
	Rectangle src = { 0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dst = { x, y, width, height };
	if (!lookingRight)
	{
		src = { 0.f, 0.f, static_cast<float>(-texture.width), static_cast<float>(texture.height) };
	}
	Vector2 origin = { 0.f, 0.f};
	
	float rotation = 0.0f;
	DrawTexturePro(texture, src, dst, origin, rotation, WHITE);

	Color hitboxColor = YELLOW;
	if (isColliding)
	{
	hitboxColor = RED;
	}

	hitboxColor.a = 50;
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), hitboxColor);
	
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		if (spellbook[i] == nullptr)
		{
			continue;
		}
		spellbook[i]->Render();
	}
	
}

void Player::SpellCollisionCheck(DynamicEntity* other)
{
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		if (spellbook[i] == nullptr)
		{
			continue;
		}
		spellbook[i]->CollisionCheck(other);
	}
}

void Player::UpdateTextureFacing()
{
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *_cam);
	if (mousePos.x >= GetCenter().x)
	{
		lookingRight = true;
		return;
	}
	
	lookingRight = false;
	
}

void Player::ClassSetupArcanist()
{
	texture = LoadTexture("Resources/Texture/Arcanist.png");
	Texture2D basicTxr = LoadTexture("Resources/Texture/ArcaneBolt.png");

	RangedBasicAttack* basicAttack = new RangedBasicAttack();
	basicAttack->SetCamera(*_cam);
	basicAttack->SetTexture(basicTxr);
	spellbook[0] = basicAttack;

	ArcanistSignature* signatureSpell = new ArcanistSignature(*this);
	spellbook[1] = signatureSpell;

	SetMaxHealth(75);
	SetHealth(75);
}

void Player::ClassSetupSummoner()
{
	texture = LoadTexture("Resources/Texture/Summoner.png");

	SummonerSignature* signatureSpell = new SummonerSignature();
	signatureSpell->SetCamera(*_cam);
	spellbook[1] = signatureSpell;

	SetMaxHealth(75);
	SetHealth(75);
}

void Player::ClassSetupNecromancer(std::vector<EnemyCorpse>& corpseList)
{
	texture = LoadTexture("Resources/Texture/Necromancer.png");
	Texture2D basicTxr = LoadTexture("Resources/Texture/FireBolt.png");

	RangedBasicAttack* basicAttack = new RangedBasicAttack();
	basicAttack->SetCamera(*_cam);
	basicAttack->SetTexture(basicTxr);
	spellbook[0] = basicAttack;

	NecromancerSignature* signatureSpell = new NecromancerSignature();
	signatureSpell->SetCorpseList(corpseList);
	spellbook[1] = signatureSpell;

	SetMaxHealth(75);
	SetHealth(75);
}

void Player::ClassSetupEnchanter()
{
	texture = LoadTexture("Resources/Texture/Enchanter.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	spellbook[0] = basicAttack;

	SetMaxHealth(100);
	SetHealth(100);
}

void Player::ClassSetupRogue()
{
	texture = LoadTexture("Resources/Texture/Rogue.png");
	Texture2D swordTexture = LoadTexture("Resources/Texture/Sword.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	basicAttack->SetTexture(swordTexture);
	spellbook[0] = basicAttack;

	RogueSignature* signatureSpell = new RogueSignature();
	spellbook[1] = signatureSpell;

	SetMaxHealth(50);
	SetHealth(50);
}

void Player::ClassSetupPaladin()
{
	texture = LoadTexture("Resources/Texture/Paladin.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	spellbook[0] = basicAttack;

	PaladinSignature* signatureSpell = new PaladinSignature();
	spellbook[1] = signatureSpell;

	SetMaxHealth(150);
	SetHealth(150);
}

void Player::Control(float deltaTime)
{
	
	isRunning = false;


	if (IsKeyDown(KEY_W))
	{
		velY-= acceleration * deltaTime;
		isRunning = true;
	}
	if (IsKeyDown(KEY_S))
	{
		velY+= acceleration * deltaTime;
		isRunning = true;
	}

	if (IsKeyDown(KEY_A))
	{
		velX-= acceleration * deltaTime;
		isRunning = true;
		
	}
	if (IsKeyDown(KEY_D))
	{
		velX+= acceleration * deltaTime;
		isRunning = true;
	
	}
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) // Basic Attack
	{
		if (spellbook[0] == nullptr)
		{
			return;
		}
		spellbook[0]->Activate(*this);
	}
	if (IsKeyPressed(KEY_SPACE)) // Signature Spell
	{
		if (spellbook[1] == nullptr)
		{
			return;
		}
		spellbook[1]->Activate(*this);
	}
}

void Player::ReduceVelocity(float deltaTime)
{

	if (velX <= 0.001f && velX >= -0.001f)
	{
		velX = 0.f;
	}
	if (velY <= 0.001f && velY >= -0.001f)
	{
		velY = 0.f;
	}
	

	float retardation = 10.f;

	if (velX > 0.f)
	{
		velX -= deltaTime * retardation;
	}
	if (velX < 0.f)
	{
		velX += deltaTime * retardation;
	}


	if (velY > 0.f)
	{
		velY -= deltaTime * retardation;
	}
	if (velY < 0.f)
	{
		velY += deltaTime * retardation;
	}
}

void Player::Movement(float deltaTime)
{

	velX /= sqrt(2.f);
	velY /= sqrt(2.f);

	x += velX * deltaTime * speed;
	y += velY * deltaTime * speed;
	
}

void Player::UpdateSpellBook(float deltaTime)
{
	
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		if (spellbook[i] == nullptr)
		{
			continue;
		}
		spellbook[i]->Update(deltaTime);
	}
}
