#include "dungeon_generation.h"

void DungeonManager::GenerateDungeon()
{
	CreateRooms();
	//FixRoomPosition();
	PlaceHallways();
	AddBonusStuff();
	FillRooms();

}

void DungeonManager::CreateRooms()
{
	float centerX = config.screenWidth / 2.f;
	float centerY = config.screenHeight / 2.f;

	SpawnRoom* spawn = new SpawnRoom();
	spawn->Setup(centerX, centerY, shortSide, shortSide);
	roomList.push_back(spawn);

	

	for (int i = 0; i < roomAmount; i++)
	{
		EnemyRoom* newRoom = new EnemyRoom();
		newRoom->Setup(centerX, centerY, longSide, shortSide);
		newRoom->SetRandomColor();
		roomList.push_back(newRoom);
	}

	BossRoom* boss = new BossRoom();
	boss->Setup(centerX, centerY, longSide, longSide);
	roomList.push_back(boss);

	
}

void DungeonManager::FixRoomPosition() // might be useless
{
	// Get stuck in loop
	bool isFixingPos = true;
	while (isFixingPos)
	{
		isFixingPos = false;
		for (int i = 0; i < roomList.size(); i++)
		{
			for (int j = 0; j < roomList.size(); j++)
			{
				if (i != j && roomList.at(i)->GenCollisionCheck(roomList.at(j)))
				{
					isFixingPos = true;
					// push rooms from each other
				}
			}
		}
	}
}

void DungeonManager::PlaceHallways()
{


	for (int i = 0; i < roomList.size(); i++)
	{
		if (i + 1 >= roomList.size())
		{
			// Boss Room
			// Connect to the room futherst away from spawn
			continue;
		}
		if (i + 2 >= roomList.size())
		{
			roomList.at(i)->FindFutherestRoom(roomList)->GenPlaceBossRoom(roomList.back(), hallwayList);
			continue;
		}
		
		
		
		roomList.at(i)->GenSetNeighbor(roomList, i, hallwayList);
	}

	for (int i = 0; i < hallwayList.size(); i++)
	{

	}

}

void DungeonManager::FillRooms()
{
	for (int i = 0; i < roomList.size(); i++)
	{
		roomList.at(i)->FillContent();
	}

	for (int i = 0; i < hallwayList.size(); i++)
	{
		if (hallwayList.at(i) == nullptr)
		{
			continue;
		}
		hallwayList.at(i)->FillContent();
	}

}

void DungeonManager::AddBonusStuff()
{
	// Add Sewers
	// Add Deity Room

	// Take a leaf room(room with only one hallway) and
}

void DungeonManager::Render()
{

	for (int i = 0; i < roomList.size(); i++)
	{
		roomList.at(i)->Render();
	}
	for (int i = 0; i < hallwayList.size(); i++)
	{
		if (hallwayList.at(i) == nullptr)
		{
			continue;
		}
		hallwayList.at(i)->Render();
	}
}

void DungeonManager::CollisionCheck(Entity* entity)
{
	for (int i = 0; i < roomList.size(); i++)
	{
		roomList.at(i)->CollisionCheck(entity);
	}

	for (int i = 0; i < hallwayList.size(); i++)
	{
		if (hallwayList.at(i) == nullptr)
		{
			continue;
		}
		hallwayList.at(i)->CollisionCheck(entity);
	}
}

void DungeonManager::AddEnemiesToDungeon(std::vector<Enemy*> enemyList)
{
	/*
	int enemies = enemyList.size();//static_cast<int>(enemyList.size() / (roomList.size() -2));
	int enemyIndex = 0;
	for (int i = 0; i < enemies; i++)
	{
		for (int j = 1; j < static_cast<int>(roomList.size() - 1); j++)
		{
			enemyList.at(enemyIndex)->SetPosition(roomList.at(j)->GetCenter());
			enemyIndex++;
		}
	
	}
	*/
	int availEnemy = static_cast<int>(enemyList.size());
	int enemyIndex = 0;
	while (availEnemy > 0)
	{

		for (int i = 0; i < roomList.size(); i++)
		{
			if (availEnemy <= 0)
			{
				break;
			}
			enemyList.at(enemyIndex)->SetPosition(roomList.at(i)->GetCenter());
			enemyIndex++;
			availEnemy--;
		}
	}
}

DungeonManager::~DungeonManager()
{
	for (int i = 0; i < roomList.size(); i++)
	{
		delete roomList.at(i);
		roomList.at(i) = nullptr;
	}
	for (int i = 0; i < hallwayList.size(); i++)
	{
		delete hallwayList.at(i);
		hallwayList.at(i) = nullptr;
	}
}
/*
void DungeonManager::GenSetNeighbor(int index)
{
	int randNum = rand() % 2 + 1;
	if (randNum + index >= roomList.size() - 1)
	{
		randNum--;
	}


	for (int i = 0; i < randNum; i++)
	{
		// Get entryPoint pos // hallway goes to parallel sides "Right wall goes to a Left wall"
		EntryPoint result;
		Hallway* newHallway = nullptr;
		int currentIndex = index + i + 1;
		bool isSearching = true;
		while (isSearching)
		{
			isSearching = false;
			if (!GenGetAvailableEntryPoints(roomList.at(currentIndex), result))
			{
				continue;
			}
			newHallway = new Hallway();
			newHallway->Setup(result, roomList.at(index), roomList.at(currentIndex), 0);
			// check if owner2 collides with any of the taken rooms, if oncollision, redo
			if (roomList.at(currentIndex)->GenListCollisionCheck(roomList, currentIndex))
			{
				isSearching = true;
				delete newHallway;
				newHallway = nullptr;
			}
		}
		if (newHallway == nullptr)
		{
			continue;
		}
		roomList.at(index)->SetIsTaken(true);
		roomList.at(currentIndex)->SetIsTaken(true);
		roomList.at(index)->neighborHalls.push_back(newHallway);
		roomList.at(currentIndex)->neighborHalls.push_back(newHallway);
		hallwayList.push_back(newHallway);
	}

}
*/
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

void Room::Setup(float xPos, float yPos, float l, float s)
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
}

bool Room::GenCollisionCheck(Room* other)
{
	
	if (x  < other->x + other->width  && x + width  > other->x  &&
		y  < other->y + other->height  && y + height  > other->y )
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

void Room::GenSetNeighbor(std::vector<Room*> &list, int index, std::vector<Hallway*>& hallList)
{
	int randNum = rand() % 2 + 1;
	if (randNum + index >= list.size() -1)
	{
		randNum--;
	}
	
	
	for (int i = 0; i < randNum; i++)
	{
		// Get entryPoint pos // hallway goes to parallel sides "Right wall goes to a Left wall"
		EntryPoint result;
		Hallway* newHallway = nullptr;
		int currentIndex = index + i + 1;
		bool isSearching = true;
		while (isSearching)
		{
			isSearching = false;
			if (!GenGetAvailableEntryPoints(list.at(currentIndex), result))
			{
				continue;
			}
			newHallway = new Hallway();
			newHallway->Setup(result, this, list.at(currentIndex), 0);
			// check if owner2 collides with any of the taken rooms, if oncollision, redo
			if (list.at(currentIndex)->GenListCollisionCheck(list, currentIndex))
			{
				isSearching = true;
				delete newHallway;
				newHallway = nullptr;
			}
		}
		if (newHallway == nullptr)
		{
			continue;
		}
		SetIsTaken(true);
		list.at(currentIndex)->SetIsTaken(true);
		neighborHalls.push_back(newHallway);
		list.at(currentIndex)->neighborHalls.push_back(newHallway);
		hallList.push_back(newHallway);
	}

}

bool Room::GenGetAvailableEntryPoints(Room* other, EntryPoint& result)
{

	
	if (other->GetIsTaken() 
		|| !entries[0].isAvailable
		&& !entries[1].isAvailable
		&& !entries[2].isAvailable
		&& !entries[3].isAvailable
		|| !other->entries[0].isAvailable
		&& !other->entries[1].isAvailable
		&& !other->entries[2].isAvailable
		&& !other->entries[3].isAvailable
		)
	{
		result.isAvailable = false;
		return false;
	}
	bool isSearching = true;
	while (isSearching)
	{

		int randNum = rand() % 4 + 1;
		switch (randNum)
		{
		case 1:
			if (entries[0].isAvailable && other->entries[1].isAvailable)
			{
				result.side = entries[0].side;
				result.position = entries[0].position;
				entries[0].isAvailable = false;
				other->entries[1].isAvailable = false;
				isSearching = false;
			}
			break;
		case 2:
			if (entries[1].isAvailable && other->entries[0].isAvailable)
			{
				result.side = entries[1].side;
				result.position = entries[1].position;
				entries[1].isAvailable = false;
				other->entries[0].isAvailable = false;
				isSearching = false;
			}
			break;
		case 3:
			if (entries[2].isAvailable && other->entries[3].isAvailable)
			{
				result.side = entries[2].side;
				result.position = entries[2].position;
				entries[2].isAvailable = false;
				other->entries[3].isAvailable = false;
				isSearching = false;
			}
			break;
		case 4:
			if (entries[3].isAvailable && other->entries[2].isAvailable)
			{
				result.side = entries[3].side;
				result.position = entries[3].position;
				entries[3].isAvailable = false;
				other->entries[2].isAvailable = false;
				isSearching = false;
			}
			break;
		}

	}
	result.isAvailable = true;
	return true;
}

void Room::GenPlaceBossRoom(Room* bossRoom, std::vector<Hallway*>& hallList)
{
	EntryPoint result;
	GenGetAvailableEntryPoints(bossRoom, result);

	Hallway* newHallway = new Hallway();
	newHallway->Setup(result, this, bossRoom, config.HALLWAY_LENGTH);
	SetIsTaken(true);
	bossRoom->SetIsTaken(true);
	neighborHalls.push_back(newHallway);
	bossRoom->neighborHalls.push_back(newHallway);
	hallList.push_back(newHallway);
	// Find suitable side of this room
	// Create Hallway on the established side
	// place bossRoom on the otherSide
}

Room* Room::FindFutherestRoom(std::vector<Room*>& list)
{
	short int closestRoom = 1;
	for (short int i = 1; i < list.size(); i++)
	{
		
		if (GetDistance(list.at(i)) < GetDistance(list.at(closestRoom)))
		{
			closestRoom = i;
		}
	}
	return list.at(closestRoom);
}

float Room::GetDistance(Room* room)
{
	float vX = GetCenter().x - room->GetCenter().x;
	float vY = GetCenter().y - room->GetCenter().y;
	return static_cast<float>(sqrt((vX * vX) + (vY * vY)));
}

Vector2 Room::GetCenter()
{
	return { (x + (width / 2.f)), (y + (height / 2.f)) };
}

bool Room::GenListCollisionCheck(std::vector<Room*>& list, int ignoreIndex)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (i == ignoreIndex || !list.at(i)->GetIsTaken())
		{
			continue;
		}
		if (GenCollisionCheck(list.at(i)))
		{
			return true;
		}
	}
	return false;
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

void Room::FillContent()
{
	// Add 4 collision boxes on each side
	// Upper Room Collision
	float thickness = 50.f;
	if (!entries[0].isAvailable)
	{
		// Get correct hallway
		int correctIndex = GetCorrectHallway(SIDE::UP, SIDE::DOWN);
		/*
		for (int i = 0; i < neighborHalls.size(); i++)
		{
			if (neighborHalls.at(i) == nullptr)
			{
				continue;
			}
			if (neighborHalls.at(i)->GetSide() == SIDE::UP && this == neighborHalls.at(i)->GetOwnerOne() 
				|| neighborHalls.at(i)->GetSide() == SIDE::DOWN && this == neighborHalls.at(i)->GetOwnerTwo())
			{
				correctIndex = i;
			}
		}
		*/
		float hallwayX = neighborHalls.at(correctIndex)->GetX();
		float hallwayWidth = neighborHalls.at(correctIndex)->GetWidth();

			// left of hallway
		Rectangle upperBoxOne{ x, y, hallwayX - x, thickness };
		collisionWalls.push_back(upperBoxOne);

		// right of hallway
		Rectangle upperBoxTwo{ hallwayX + hallwayWidth, y, width - (hallwayX - x + hallwayWidth), thickness};
		collisionWalls.push_back(upperBoxTwo);
	}
	else
	{
		Rectangle upperBox{ x, y, width, thickness };
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
		Rectangle leftBoxOne{ x, y, thickness, hallwayY - y};
		collisionWalls.push_back(leftBoxOne);

		// right of hallway
		Rectangle leftBoxTwo{ x, hallwayY + hallwayHeight, thickness, height - (hallwayY - y + hallwayHeight) };
		collisionWalls.push_back(leftBoxTwo);
	}
	else
	{
		Rectangle leftBox{ x, y, thickness, height };
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

// Hallway
void Hallway::Setup(EntryPoint src, Room* ownerOne, Room* ownerTwo, float bonusLen)
{
	owners[0] = ownerOne;
	owners[1] = ownerTwo;

	switch(src.side)
	{
	case SIDE::UP:
		height = config.HALLWAY_LENGTH + bonusLen;
		width = config.HALLWAY_LENGTH / 2.f;
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3)); ;//x = ownerOne->x - src.position.x + (ownerOne->width / 2.f);
		y = ownerOne->y - src.position.y - height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); // (ownerTwo->width / 2.f) + (width / 2.f);//x - (ownerTwo->width / 2.f) + rand() % static_cast<int>(ownerTwo->width - (width * 2)) - static_cast<int>(((ownerTwo->width / 2.0f )-width)); // x - (owner2width /2) + width TO x + (owner2width /2) - width
		ownerTwo->y = y - ownerTwo->height;
		break;
	case SIDE::DOWN:
		height = config.HALLWAY_LENGTH + bonusLen;
		width = config.HALLWAY_LENGTH / 2.f;
		x = (ownerOne->x + width) + rand() % static_cast<int>(ownerOne->width - (width * 3)); // x = ownerOne->x - src.position.x + (ownerOne->width / 2.f);
		y = ownerOne->y + src.position.y + ownerOne->height;

		ownerTwo->x = (x - width) - rand() % static_cast<int>(ownerTwo->width - (width * 3)); //ownerTwo->x = x - (ownerTwo->width / 2.f) + rand() % static_cast<int>(ownerTwo->width - (width * 2)) - static_cast<int>(((ownerTwo->width / 2.0f)- width));
		ownerTwo->y = y + height;
		break;
	case SIDE::LEFT:
		height = config.HALLWAY_LENGTH / 2.f;
		width = config.HALLWAY_LENGTH + bonusLen;
		x = ownerOne->x - src.position.x - width;
		y = ownerOne->y - src.position.y + (ownerOne->height / 2.f);

		ownerTwo->x = x - ownerTwo->width;
		ownerTwo->y = (y - height) - rand() % static_cast<int>(ownerTwo->height - (height * 3));  //ownerTwo->y = y - (ownerTwo->height / 2.f) + rand() % static_cast<int>(ownerTwo->height - (height * 2)) - static_cast<int>((ownerTwo->height / 2.0f));
		break;
	case SIDE::RIGHT:
		height = config.HALLWAY_LENGTH / 2.f;
		width = config.HALLWAY_LENGTH + bonusLen;
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

void Hallway::Render()
{
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
	
	for (int i = 0; i < collisionWalls.size(); i++)
	{
		DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), YELLOW);
	}
	
	if (owners[0] == nullptr || owners[1] == nullptr)
	{
		return;
	}
	// owners are the same
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
		Rectangle top = {x, y, width, thickness};
		collisionWalls.push_back(top);
		Rectangle bot = {x, y + height, width, thickness };
		collisionWalls.push_back(bot);
		return;
	}
	Rectangle left = {x, y, thickness, height };
	collisionWalls.push_back(left);
	Rectangle right = {x + width, y, thickness, height };
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
