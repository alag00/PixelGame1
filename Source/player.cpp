#include "player.h"

Player::Player(int role, Camera2D& cam)
{
	_cam = &cam;
	// Load Textures
	switch (role)
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
		ClassSetupNecromancer();
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
		

	//offsetTextureX = (playerTexture.width * config.PIXEL_SCALE) / 2.f;
	//offsetTextureY = (playerTexture.height * config.PIXEL_SCALE) / 2.f;

	width = texture.width * config.PIXEL_SCALE;
	height = texture.height * config.PIXEL_SCALE;
	
}

Player::~Player()
{
	
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		delete spellbook[i];
		spellbook[i] = nullptr;
	}
	
}
/*
void Player::SetStartPosition(Vector2 pos)
{
	x = pos.x;
	y = pos.y;
}
*/

void Player::Update(float deltaTime)
{
	isColliding = false;
	Control();
	Movement(deltaTime);
	UpdateSpellBook(deltaTime);
}

void Player::OnCollision(Entity* other)
{
	// move away
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
	Rectangle dst = { x, y, static_cast<float>(texture.width * config.PIXEL_SCALE), static_cast<float>(texture.height * config.PIXEL_SCALE)};
	Vector2 origin = { 0.f, 0.f};
	//Vector2 origin = { static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f) };
	float rotation = 0.0f;

	DrawTexturePro(texture, src, dst, origin, rotation, WHITE);

	// CollisionBox
	
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

void Player::ClassSetupArcanist()
{
	texture = LoadTexture("Resources/Arcanist.png");
	Texture2D basicTxr = LoadTexture("Resources/ArcaneBolt.png");

	RangedBasicAttack* basicAttack = new RangedBasicAttack();
	basicAttack->SetCamera(*_cam);
	basicAttack->SetTexture(basicTxr);
	spellbook[0] = basicAttack;

	ArcanistSignature* signatureSpell = new ArcanistSignature(*this);
	spellbook[1] = signatureSpell;

	SetHealth(75);
}

void Player::ClassSetupSummoner()
{
	texture = LoadTexture("Resources/Summoner.png");

	SummonerSignature* signatureSpell = new SummonerSignature();
	signatureSpell->SetCamera(*_cam);
	spellbook[1] = signatureSpell;

	SetHealth(75);
}

void Player::ClassSetupNecromancer()
{
	texture = LoadTexture("Resources/Necromancer.png");
	Texture2D basicTxr = LoadTexture("Resources/FireBolt.png");

	RangedBasicAttack* basicAttack = new RangedBasicAttack();
	basicAttack->SetCamera(*_cam);
	basicAttack->SetTexture(basicTxr);
	spellbook[0] = basicAttack;

	SetHealth(75);
}

void Player::ClassSetupEnchanter()
{
	texture = LoadTexture("Resources/Enchanter.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	spellbook[0] = basicAttack;

	SetHealth(100);
}

void Player::ClassSetupRogue()
{
	texture = LoadTexture("Resources/Rogue.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	spellbook[0] = basicAttack;

	RogueSignature* signatureSpell = new RogueSignature();
	spellbook[1] = signatureSpell;

	SetHealth(50);
}

void Player::ClassSetupPaladin()
{
	texture = LoadTexture("Resources/Paladin.png");

	MeleeBasicAttack* basicAttack = new MeleeBasicAttack();
	basicAttack->SetCamera(*_cam);
	spellbook[0] = basicAttack;

	PaladinSignature* signatureSpell = new PaladinSignature();
	spellbook[1] = signatureSpell;

	SetHealth(150);
}

void Player::Control()
{
	velX = 0.f;
	velY = 0.f;
	isRunning = false;


	if (IsKeyPressed(KEY_P))
	{
		TakeDamage(15);
	}

	if (IsKeyDown(KEY_W))
	{
		velY-= 1.f;
		isRunning = true;
	}
	if (IsKeyDown(KEY_S))
	{
		velY+= 1.f;
		isRunning = true;
	}

	if (IsKeyDown(KEY_A))
	{
		velX-= 1.f;
		isRunning = true;
	}
	if (IsKeyDown(KEY_D))
	{
		velX+= 1.f;
		isRunning = true;
	}

	if (isRunning && speed <= maxSpeed)
	{
		speed += acceleration;
	}
	else if (!isRunning)
	{
		speed = startSpeed;
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

void Player::Movement(float deltaTime)
{
	/**
	float length = static_cast<float>(sqrt((velX * velX) + (velY * velY)));

	velX = (velX / length);
	velY = (velY / length);

	*/
	if (velX != 0.f && velY != 0.f)
	{
		// moving diagonally
		velX /= sqrt(2.f);
		velY /= sqrt(2.f);
	}
	
	x += velX * deltaTime *  speed;
	y += velY * deltaTime * speed;
	
}

void Player::UpdateSpellBook(float deltaTime)
{
	//spellbook[0]->Update(deltaTime);
	//spellbook[1]->Update(deltaTime);
	
	for (int i = 0; i < MAX_SPELL_SLOTS; i++)
	{
		if (spellbook[i] == nullptr)
		{
			continue;
		}
		spellbook[i]->Update(deltaTime);
	}
}
