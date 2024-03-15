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
		//height = config.HALLWAY_LENGTH + bonusLen;
		//width = config.HALLWAY_LENGTH / 2.f;
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3)); ;//x = ownerOne->x - src.position.x + (ownerOne->width / 2.f);
		y = ownerOne->y - src.position.y - height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); // (ownerTwo->width / 2.f) + (width / 2.f);//x - (ownerTwo->width / 2.f) + rand() % static_cast<int>(ownerTwo->width - (width * 2)) - static_cast<int>(((ownerTwo->width / 2.0f )-width)); // x - (owner2width /2) + width TO x + (owner2width /2) - width
		ownerTwo->y = y - ownerTwo->height;
		break;
	case SIDE::DOWN:
		//height = config.HALLWAY_LENGTH + bonusLen;
		//width = config.HALLWAY_LENGTH / 2.f;
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3)); // x = ownerOne->x - src.position.x + (ownerOne->width / 2.f);
		y = ownerOne->y + src.position.y + ownerOne->height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); //ownerTwo->x = x - (ownerTwo->width / 2.f) + rand() % static_cast<int>(ownerTwo->width - (width * 2)) - static_cast<int>(((ownerTwo->width / 2.0f)- width));
		ownerTwo->y = y + height;
		break;
	case SIDE::LEFT:
		//height = config.HALLWAY_LENGTH / 2.f;
		//width = config.HALLWAY_LENGTH + bonusLen;
		x = ownerOne->x - src.position.x - width;
		y = ownerOne->y - src.position.y + (ownerOne->height / 2.f);

		ownerTwo->x = x - ownerTwo->width;
		ownerTwo->y = (y - height) - rand() % static_cast<int>(ownerTwo->height - (height * 3));  //ownerTwo->y = y - (ownerTwo->height / 2.f) + rand() % static_cast<int>(ownerTwo->height - (height * 2)) - static_cast<int>((ownerTwo->height / 2.0f));
		break;
	case SIDE::RIGHT:
		//height = config.HALLWAY_LENGTH / 2.f;
		//width = config.HALLWAY_LENGTH + bonusLen;
		x = ownerOne->x + src.position.x + ownerOne->width;
		y = ownerOne->y - src.position.y + (ownerOne->height / 2.f);

		ownerTwo->x = x + width;
		ownerTwo->y = (y - height) - rand() % static_cast<int>(ownerTwo->height - (height * 3)); // ownerTwo->y = y - (ownerTwo->height / 2.f) + rand() % static_cast<int>(ownerTwo->height - (height * 2)) - static_cast<int>((ownerTwo->height / 2.0f));
		break;
	}
	side = src.side;
	//src.position = { x, y };
	//ownerTwo->GenSetPosFromEntryPoints(src);
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
	//Vector2 origin = { static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f) };
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
	// Add 2 collision boxes on the long sides
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

void Hallway::CollisionCheck(Entity* entity)// check if touching entity
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
void Hallway::OnCollision(Entity* entity, Rectangle wall)// push entity back 
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

	//DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

	Rectangle src = { 0.f, 0.f, static_cast<float>(txr.width), static_cast<float>(txr.height) };
	Rectangle dst = { x, y, static_cast<float>(txr.width * config.PIXEL_SCALE), static_cast<float>(txr.height * config.PIXEL_SCALE) };
	Vector2 origin = { 0.f, 0.f };
	//Vector2 origin = { static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f), static_cast<float>((texture.width * config.PIXEL_SCALE) / 2.0f) };
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
	/*
	int randNum = rand() % 2 + 1;
	switch (randNum)
	{
	case 1:
		width = l + rand() % (100 * static_cast<int>(config.PIXEL_SCALE)) + 1;
		height = s + rand() % (100 * static_cast<int>(config.PIXEL_SCALE)) + 1;
		break;
	case 2:
		width = s + rand() % (100 * static_cast<int>(config.PIXEL_SCALE)) + 1;
		height = l + rand() % (100 * static_cast<int>(config.PIXEL_SCALE)) + 1;
		break;
	}
	*/
	//xPos;
	//yPos;
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
	// Add 4 collision boxes on each side
	// Upper Room Collision
	float thickness = 50.f;
	CreateCollisionWalls(entries[0], thickness, SIDE::UP, SIDE::DOWN);
	CreateCollisionWalls(entries[1], thickness, SIDE::DOWN, SIDE::UP);
	CreateCollisionWalls(entries[2], thickness, SIDE::LEFT, SIDE::RIGHT);
	CreateCollisionWalls(entries[3], thickness, SIDE::RIGHT, SIDE::LEFT);
	/*
	if (!entries[0].isAvailable)
	{
		// Get correct hallway
		int correctIndex = GetCorrectHallway(SIDE::UP, SIDE::DOWN);

		float hallwayX = neighborHalls.at(correctIndex)->GetX();
		float hallwayWidth = neighborHalls.at(correctIndex)->GetWidth();

		// left of hallway
		Rectangle upperBoxOne{ x, y - thickness, hallwayX - x, thickness };
		collisionWalls.push_back(upperBoxOne);

		// right of hallway
		Rectangle upperBoxTwo{ hallwayX + hallwayWidth, y - thickness, width - (hallwayX - x + hallwayWidth), thickness };
		collisionWalls.push_back(upperBoxTwo);
	}
	else
	{
		Rectangle upperBox{ x, y - thickness, width, thickness };
		collisionWalls.push_back(upperBox);
	}

	if (!entries[1].isAvailable)
	{
		// Get correct hallway
		int correctIndex = 0;

		for (int i = 0; i < neighborHalls.size(); i++)
		{
			if (neighborHalls.at(i) == nullptr)
			{
				continue;
			}
			if (neighborHalls.at(i)->GetSide() == SIDE::DOWN && this == neighborHalls.at(i)->GetOwnerOne()
				|| neighborHalls.at(i)->GetSide() == SIDE::UP && this == neighborHalls.at(i)->GetOwnerTwo())
			{
				correctIndex = i;
			}
		}

		float hallwayX = neighborHalls.at(correctIndex)->GetX();
		float hallwayWidth = neighborHalls.at(correctIndex)->GetWidth();

		// left of hallway
		Rectangle bottomBoxOne{ x, y + height, hallwayX - x, thickness };
		collisionWalls.push_back(bottomBoxOne);

		// right of hallway
		Rectangle bottomBoxTwo{ hallwayX + hallwayWidth, y + height, width - (hallwayX - x + hallwayWidth), thickness };
		collisionWalls.push_back(bottomBoxTwo);
	}
	else
	{
		Rectangle bottomBox{ x, y + height, width, thickness };
		collisionWalls.push_back(bottomBox);
	}

	if (!entries[2].isAvailable)
	{
		// Get correct hallway
		int correctIndex = 0;

		for (int i = 0; i < neighborHalls.size(); i++)
		{
			if (neighborHalls.at(i) == nullptr)
			{
				continue;
			}
			if (neighborHalls.at(i)->GetSide() == SIDE::LEFT && this == neighborHalls.at(i)->GetOwnerOne()
				|| neighborHalls.at(i)->GetSide() == SIDE::RIGHT && this == neighborHalls.at(i)->GetOwnerTwo())
			{
				correctIndex = i;
			}
		}

		float hallwayY = neighborHalls.at(correctIndex)->GetY();
		float hallwayHeight = neighborHalls.at(correctIndex)->GetHeight();

		// left of hallway
		Rectangle leftBoxOne{ x - thickness, y, thickness, hallwayY - y };
		collisionWalls.push_back(leftBoxOne);

		// right of hallway
		Rectangle leftBoxTwo{ x - thickness, hallwayY + hallwayHeight, thickness, height - (hallwayY - y + hallwayHeight) };
		collisionWalls.push_back(leftBoxTwo);
	}
	else
	{
		Rectangle leftBox{ x - thickness, y, thickness, height };
		collisionWalls.push_back(leftBox);
	}

	if (!entries[3].isAvailable)
	{
		// Get correct hallway
		int correctIndex = 0;

		for (int i = 0; i < neighborHalls.size(); i++)
		{
			if (neighborHalls.at(i) == nullptr)
			{
				continue;
			}
			if (neighborHalls.at(i)->GetSide() == SIDE::RIGHT && this == neighborHalls.at(i)->GetOwnerOne()
				|| neighborHalls.at(i)->GetSide() == SIDE::LEFT && this == neighborHalls.at(i)->GetOwnerTwo())
			{
				correctIndex = i;
			}
		}

		float hallwayY = neighborHalls.at(correctIndex)->GetY();
		float hallwayHeight = neighborHalls.at(correctIndex)->GetHeight();

		// left of hallway
		Rectangle leftBoxOne{ x + width, y, thickness, hallwayY - y };
		collisionWalls.push_back(leftBoxOne);

		// right of hallway
		Rectangle leftBoxTwo{ x + width, hallwayY + hallwayHeight, thickness, height - (hallwayY - y + hallwayHeight) };
		collisionWalls.push_back(leftBoxTwo);
	}
	else
	{
		Rectangle rightBox{ x + width, y, thickness, height };
		collisionWalls.push_back(rightBox);
	}
	*/
}

void Room::CollisionCheck(Entity* entity)// check if touching entity
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

void Room::OnCollision(Entity* entity, Rectangle wall)// push entity back 
{
	// Push only in 1 axis
	// if wall width is longer only push on y-axis
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