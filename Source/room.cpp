#include "room.h"


// Hallway
void Hallway::Setup(EntryPoint src, Room* ownerOne, Room* ownerTwo, Texture2D newTxr)
{
	owners[0] = ownerOne;
	owners[1] = ownerTwo;

	txr = newTxr;
	width = txr.width * config.PIXEL_SCALE;
	height = txr.height * config.PIXEL_SCALE;

	switch (src.side)
	{
	case SIDE::UP:
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3)); 
		y = ownerOne->y - src.position.y - height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); 
		ownerTwo->y = y - ownerTwo->height;
		break;
	case SIDE::DOWN:
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3));
		y = ownerOne->y + src.position.y + ownerOne->height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); 
		ownerTwo->y = y + height;
		break;
	case SIDE::LEFT:
		x = ownerOne->x - src.position.x - width;
		y = ownerOne->y - src.position.y + (ownerOne->height / 2.f);

		ownerTwo->x = x - ownerTwo->width;
		ownerTwo->y = (y - height) - rand() % static_cast<int>(ownerTwo->height - (height * 3));  
		break;
	case SIDE::RIGHT:
		x = ownerOne->x + src.position.x + ownerOne->width;
		y = ownerOne->y - src.position.y + (ownerOne->height / 2.f);

		ownerTwo->x = x + width;
		ownerTwo->y = (y - height) - rand() % static_cast<int>(ownerTwo->height - (height * 3)); 
		break;
	}
	side = src.side;
}

void Hallway::Render(Rectangle cam)
{
	if (!(x  < cam.x + cam.width
		&& x + width  > cam.x
		&& y  < cam.y + cam.height
		&& y + height  > cam.y))
	{
		return;
	}

	Color color = WHITE;
	switch (side)
	{
	case SIDE::UP:
		color = PINK;
		break;
	case SIDE::DOWN:
		color = GREEN;
		break;
	case SIDE::LEFT:
		color = RED;
		break;
	case SIDE::RIGHT:
		color = BLUE;
		break;
	}
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

	Rectangle src = { 0.f, 0.f, static_cast<float>(txr.width), static_cast<float>(txr.height) };
	Rectangle dst = { x, y, static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	
	float rotation = 0.0f;

	DrawTexturePro(txr, src, dst, origin, rotation, WHITE);

	Color newColor = YELLOW;
	newColor.a = 30;
	
	for (int i = 0; i < collisionWalls.size(); i++)
	{
		DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), newColor);
	}
	
	if (owners[0] == nullptr || owners[1] == nullptr)
	{
		return;
	}

	int newX = static_cast<int>(x + (width / 2.f));
	int newY = static_cast<int>(y + (height / 2.f));

	int own1X = static_cast<int>(owners[0]->x + (owners[0]->width / 2.f));
	int own1Y = static_cast<int>(owners[0]->y + (owners[0]->height / 2.f));

	int own2X = static_cast<int>(owners[1]->x + (owners[1]->width / 2.f));
	int own2Y = static_cast<int>(owners[1]->y + (owners[1]->height / 2.f));
	DrawLine(newX, newY, own1X, own1Y, color);
	DrawLine(newX, newY, own2X, own2Y, color);
}

void Hallway::FillContent()
{
	float thickness = 50.f;
	if (width > height)
	{
		Rectangle top = { x, y - thickness, width, thickness };
		collisionWalls.push_back(top);
		Rectangle bot = { x, y + height, width, thickness };
		collisionWalls.push_back(bot);
		return;
	}
	Rectangle left = { x - thickness, y, thickness, height };
	collisionWalls.push_back(left);
	Rectangle right = { x + width, y, thickness, height };
	collisionWalls.push_back(right);

}

void Hallway::CollisionCheck(Entity* entity)
{
	for (int i = 0; i < collisionWalls.size(); i++)
	{
		if (collisionWalls.at(i).x  < entity->x + entity->width
			&& collisionWalls.at(i).x + collisionWalls.at(i).width  > entity->x
			&& collisionWalls.at(i).y  < entity->y + entity->height
			&& collisionWalls.at(i).y + collisionWalls.at(i).height  > entity->y)
		{
			OnCollision(entity, collisionWalls.at(i));
		}
	}
}
void Hallway::OnCollision(Entity* entity, Rectangle wall)
{
	if (wall.width > wall.height)
	{
		if (wall.y < entity->y)
		{
			entity->y = wall.y + wall.height;
			return;
		}
		entity->y = wall.y - entity->height;
		return;
	}

	if (wall.x < entity->x)
	{
		entity->x = wall.x + wall.width;
		return;
	}
	entity->x = wall.x - entity->width;
}


// Room Stuff


Room::Room()
{
	entries[0].side = SIDE::UP;
	entries[0].position.x = width / 2.f;
	entries[0].position.y = 0.f;

	entries[1].side = SIDE::DOWN;
	entries[1].position.x = width / 2.f;
	entries[1].position.y = height;

	entries[2].side = SIDE::LEFT;
	entries[2].position.x = 0.f;
	entries[2].position.y = height / 2.f;

	entries[3].side = SIDE::RIGHT;
	entries[3].position.x = width;
	entries[3].position.y = height / 2.f;
}

Room::~Room()
{
	for (int i = 0; i < neighborHalls.size(); i++)
	{
		delete neighborHalls.at(i);
		neighborHalls.at(i) = nullptr;
	}
}

void Room::Render(Rectangle cam)
{
	if (!(x  < cam.x + cam.width
		&& x + width  > cam.x
		&& y  < cam.y + cam.height
		&& y + height  > cam.y))
	{
		return;
	}

	
	Rectangle src = { 0.f, 0.f, static_cast<float>(txr.width), static_cast<float>(txr.height) };
	Rectangle dst = { x, y, static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	
	float rotation = 0.0f;

	DrawTexturePro(txr, src, dst, origin, rotation, WHITE);
	
	Color newColor = YELLOW;
	newColor.a = 30;

	for (int i = 0; i < collisionWalls.size(); i++)
	{
		DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), newColor);
	}
	
}

void Room::Setup(float xPos, float yPos, Texture2D newTxr)
{
	
	int randX = rand() % 20 - 10;
	int randY = rand() % 20 - 10;
	while (randX == 0 && randY == 0)
	{
		randX = rand() % 20 - 10;
		randY = rand() % 20 - 10;
	}
	x = xPos * (randX * 0.1f);
	y = yPos * (randY * 0.1f);
	
	txr = newTxr;
	width = txr.width * config.PIXEL_SCALE;
	height = txr.height * config.PIXEL_SCALE;
}

bool Room::GenCollisionCheck(Room* other)
{

	if (x  < other->x + other->width && x + width  > other->x &&
		y  < other->y + other->height && y + height  > other->y)
	{
		float pushForce = 3.5f;

		float vectorX = x - other->x;
		float vectorY = y - other->y;

		float length = static_cast<float>(sqrt((vectorX * vectorX) + (vectorY * vectorY)));

		vectorX = (vectorX / length);
		vectorY = (vectorY / length);

		x += vectorX * pushForce;
		y += vectorY * pushForce;

		return true;
	}
	return false;
}

Vector2 Room::GetCenter()
{
	return { (x + (width / 2.f)), (y + (height / 2.f)) };
}

int Room::GetCorrectHallway(SIDE ownerOneSide, SIDE ownerTwoSide)
{
	int correctIndex = 0;

	for (int i = 0; i < neighborHalls.size(); i++)
	{
		if (neighborHalls.at(i) == nullptr)
		{
			continue;
		}
		if (neighborHalls.at(i)->GetSide() == ownerOneSide && this == neighborHalls.at(i)->GetOwnerOne()
			|| neighborHalls.at(i)->GetSide() == ownerTwoSide && this == neighborHalls.at(i)->GetOwnerTwo())
		{
			correctIndex = i;
		}
	}

	return correctIndex;
}

void Room::CreateCollisionWallSingle(EntryPoint entry, float thickness)
{
	Rectangle box{ 0.f, 0.f, 0.f, 0.f};
	switch (entry.side)
	{
	case SIDE::UP:
		box = { x, y - thickness, width, thickness };
		break;
	case SIDE::DOWN:
		box = { x, y + height, width, thickness };
		break;
	case SIDE::LEFT:
		box = { x - thickness , y, thickness, height};
		break;
	case SIDE::RIGHT:
		box = { x + width, y, thickness, height };
		break;
	}
	collisionWalls.push_back(box);

}
void Room::CreateCollisionWallMulti(EntryPoint entry, float thickness, int hallwayIndex )
{

	float hallwayX = neighborHalls.at(hallwayIndex)->GetX();
	float hallwayY = neighborHalls.at(hallwayIndex)->GetY();
	float hallwayWidth = neighborHalls.at(hallwayIndex)->GetWidth();
	float hallwayHeight = neighborHalls.at(hallwayIndex)->GetHeight();
	Rectangle boxOne{0.f, 0.f, 0.f, 0.f};
	Rectangle boxTwo{ 0.f, 0.f, 0.f, 0.f };
	switch (entry.side)
	{
	case SIDE::UP:
		boxOne = { x, y - thickness, hallwayX - x, thickness };
		boxTwo = { hallwayX + hallwayWidth, y - thickness, width - (hallwayX - x + hallwayWidth), thickness };
		break;
	case SIDE::DOWN:
		boxOne = { x, y + height, hallwayX - x, thickness };
		boxTwo = { hallwayX + hallwayWidth, y + height, width - (hallwayX - x + hallwayWidth), thickness };
		break;
	case SIDE::LEFT:
		boxOne = { x - thickness, y, thickness, hallwayY - y };
		boxTwo = { x - thickness, hallwayY + hallwayHeight, thickness, height - (hallwayY - y + hallwayHeight) };
		break;
	case SIDE::RIGHT:
		boxOne = { x + width, y, thickness, hallwayY - y };
		boxTwo = { x + width, hallwayY + hallwayHeight, thickness, height - (hallwayY - y + hallwayHeight) };
		break;
	}
	collisionWalls.push_back(boxOne);
	collisionWalls.push_back(boxTwo);
}

void Room::CreateCollisionWalls(EntryPoint entry, float thickness, SIDE ownerOneSide, SIDE ownerTwoSide)
{
	if (entry.isAvailable)
	{
		CreateCollisionWallSingle(entry, thickness);
		
		return;
	}
	CreateCollisionWallMulti(entry, thickness, GetCorrectHallway(ownerOneSide, ownerTwoSide));
}

void Room::FillContent()
{

	float thickness = 50.f;
	CreateCollisionWalls(entries[0], thickness, SIDE::UP, SIDE::DOWN);
	CreateCollisionWalls(entries[1], thickness, SIDE::DOWN, SIDE::UP);
	CreateCollisionWalls(entries[2], thickness, SIDE::LEFT, SIDE::RIGHT);
	CreateCollisionWalls(entries[3], thickness, SIDE::RIGHT, SIDE::LEFT);
}

void Room::CollisionCheck(Entity* entity)
{
	for (int i = 0; i < collisionWalls.size(); i++)
	{
		if (collisionWalls.at(i).x  < entity->x + entity->width
			&& collisionWalls.at(i).x + collisionWalls.at(i).width  > entity->x
			&& collisionWalls.at(i).y  < entity->y + entity->height
			&& collisionWalls.at(i).y + collisionWalls.at(i).height  > entity->y)
		{
			OnCollision(entity, collisionWalls.at(i));
		}
	}
}

void Room::OnCollision(Entity* entity, Rectangle wall)
{

	if (wall.width > wall.height)
	{
		if (wall.y < entity->y)
		{
			entity->y = wall.y + wall.height;
			return;
		}
		entity->y = wall.y - entity->height;
		return;
	}

	if (wall.x < entity->x)
	{
		entity->x = wall.x + wall.width;
		return;
	}
	entity->x = wall.x - entity->width;
}
