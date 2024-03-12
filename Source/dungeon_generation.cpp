#include "dungeon_generation.h"

void DungeonManager::GenerateDungeon()
{
	LoadTextures();
	CreateRooms();
	PlaceHallways();
	AddBonusStuff();
	FillRooms();

}

void DungeonManager::LoadTextures()
{
	// Hallway Txr
	Image hallwayImg = LoadImage("Resources/VerticalHallway.png");
	hallVertTxr = LoadTextureFromImage(hallwayImg);

	ImageRotateCW(&hallwayImg);
	ImageFlipVertical(&hallwayImg);
	hallHoriTxr = LoadTextureFromImage(hallwayImg);
	
	UnloadImage(hallwayImg);

	// Room Txr
	Image roomImg = LoadImage("Resources/RoomFloor.png");
	roomHoriTxr = LoadTextureFromImage(roomImg);

	ImageRotateCW(&roomImg);
	ImageFlipVertical(&roomImg);
	roomVertTxr = LoadTextureFromImage(roomImg);

	UnloadImage(roomImg);

	spawnTxr = LoadTexture("Resources/SpawnRoomFloor.png");
	bossTxr = LoadTexture("Resources/BossRoomFloor.png");
}

void DungeonManager::CreateRooms()
{
	float centerX = config.screenWidth / 2.f;
	float centerY = config.screenHeight / 2.f;

	SpawnRoom* spawn = new SpawnRoom();
	spawn->Setup(centerX, centerY, spawnTxr);
	roomList.push_back(spawn);

	

	for (int i = 0; i < roomAmount; i++)
	{
		EnemyRoom* newRoom = new EnemyRoom();
		int randNum = rand() % 2 + 1;
		switch (randNum)
		{
		case 1:
			newRoom->Setup(centerX, centerY, roomHoriTxr);
			break;
		case 2:
			newRoom->Setup(centerX, centerY, roomVertTxr);
			break;
		}
		
	
		//newRoom->SetRandomColor();
		roomList.push_back(newRoom);
	}

	BossRoom* boss = new BossRoom();
	boss->Setup(centerX, centerY, bossTxr);
	roomList.push_back(boss);

	
}

void DungeonManager::PlaceHallways()
{
	int bossIndex = static_cast<int>(roomList.size()-1);
	int lastIndex = bossIndex - 1;
	for (int i = 0; i < roomList.size(); i++)
	{
		if (i  >= bossIndex)
		{
			continue;
		}
		if (i >= lastIndex)
		{
			GenPlaceBossRoom(FindFutherestRoom(i), roomList.back());
			continue;
		}
		GenSetNeighbor(i);
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

void DungeonManager::Render(Rectangle cam)
{

	for (int i = 0; i < roomList.size(); i++)
	{
		roomList.at(i)->Render(cam);
	}
	for (int i = 0; i < hallwayList.size(); i++)
	{
		if (hallwayList.at(i) == nullptr)
		{
			continue;
		}
		hallwayList.at(i)->Render(cam);
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
	int availEnemy = static_cast<int>(enemyList.size() - 1);
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
	enemyList.back()->SetPosition(roomList.back()->GetCenter());
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

void DungeonManager::GenSetNeighbor(int index)
{
	int randNum = rand() % 2 + 1;
	int limitTest = randNum + index;
	if (limitTest >= roomList.size() - 1)
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
			if (!GenGetAvailableEntryPoints(roomList.at(index), roomList.at(currentIndex), result))
			{
				continue;
			}
			newHallway = new Hallway();
			if (result.side == SIDE::UP || result.side == SIDE::DOWN)
			{
				newHallway->Setup(result, roomList.at(index), roomList.at(currentIndex), hallVertTxr);
			}
			else if (result.side == SIDE::LEFT || result.side == SIDE::RIGHT)
			{
				newHallway->Setup(result, roomList.at(index), roomList.at(currentIndex), hallHoriTxr);
			}
			// check if owner2 collides with any of the taken rooms, if oncollision, redo
			if (GenListCollisionCheck(currentIndex))
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

bool DungeonManager::GenGetAvailableEntryPoints(Room* thisRoom, Room* otherRoom, EntryPoint& result)
{


	if (otherRoom->GetIsTaken()
		|| !thisRoom->entries[0].isAvailable
		&& !thisRoom->entries[1].isAvailable
		&& !thisRoom->entries[2].isAvailable
		&& !thisRoom->entries[3].isAvailable
		|| !otherRoom->entries[0].isAvailable
		&& !otherRoom->entries[1].isAvailable
		&& !otherRoom->entries[2].isAvailable
		&& !otherRoom->entries[3].isAvailable
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
			if (thisRoom->entries[0].isAvailable && otherRoom->entries[1].isAvailable)
			{
				result.side = thisRoom->entries[0].side;
				result.position = thisRoom->entries[0].position;
				thisRoom->entries[0].isAvailable = false;
				otherRoom->entries[1].isAvailable = false;
				isSearching = false;
			}
			break;
		case 2:
			if (thisRoom->entries[1].isAvailable && otherRoom->entries[0].isAvailable)
			{
				result.side = thisRoom->entries[1].side;
				result.position = thisRoom->entries[1].position;
				thisRoom->entries[1].isAvailable = false;
				otherRoom->entries[0].isAvailable = false;
				isSearching = false;
			}
			break;
		case 3:
			if (thisRoom->entries[2].isAvailable && otherRoom->entries[3].isAvailable)
			{
				result.side = thisRoom->entries[2].side;
				result.position = thisRoom->entries[2].position;
				thisRoom->entries[2].isAvailable = false;
				otherRoom->entries[3].isAvailable = false;
				isSearching = false;
			}
			break;
		case 4:
			if (thisRoom->entries[3].isAvailable && otherRoom->entries[2].isAvailable)
			{
				result.side = thisRoom->entries[3].side;
				result.position = thisRoom->entries[3].position;
				thisRoom->entries[3].isAvailable = false;
				otherRoom->entries[2].isAvailable = false;
				isSearching = false;
			}
			break;
		}

	}
	result.isAvailable = true;
	return true;
}

void DungeonManager::GenPlaceBossRoom(Room* lastRoom, Room* bossRoom)
{
	Hallway* newHallway = nullptr;
	bool isSearching = false;
	do {
		isSearching = false;

		EntryPoint result;
		result.side = SIDE::NONE;
		GenGetAvailableEntryPoints(lastRoom, bossRoom, result);

		newHallway = new Hallway();
		if (result.side == SIDE::UP || result.side == SIDE::DOWN)
		{
			newHallway->Setup(result, lastRoom, bossRoom, hallVertTxr);
		}
		else if (result.side == SIDE::LEFT || result.side == SIDE::RIGHT)
		{
			newHallway->Setup(result, lastRoom, bossRoom, hallHoriTxr);
		}
		int bossIndex = static_cast<int>(roomList.size() - 1);
		if (GenListCollisionCheck(bossIndex) || result.side == SIDE::NONE)
		{
			isSearching = true;
			delete newHallway;
			newHallway = nullptr;
		}
	} while (isSearching);

	//newHallway->Setup(result, lastRoom, bossRoom, config.HALLWAY_LENGTH);
	lastRoom->SetIsTaken(true);
	bossRoom->SetIsTaken(true);
	lastRoom->neighborHalls.push_back(newHallway);
	bossRoom->neighborHalls.push_back(newHallway);
	hallwayList.push_back(newHallway);
	// Find suitable side of this room
	// Create Hallway on the established side
	// place bossRoom on the otherSide
}

bool DungeonManager::GenListCollisionCheck(int index)
{
	for (int i = 0; i < roomList.size(); i++)
	{
		if (i == index || !roomList.at(i)->GetIsTaken())
		{
			continue;
		}
		if (roomList.at(index)->GenCollisionCheck(roomList.at(i)))
		{
			return true;
		}
	}
	return false;
}

Room* DungeonManager::FindFutherestRoom(int index)
{
	short int futherestRoom = 0;
	for (short int i = 0; i < roomList.size(); i++)
	{

		if (GetDistance(roomList.at(index), roomList.at(i)) < GetDistance(roomList.at(index), roomList.at(futherestRoom)))
		{
			futherestRoom = i;
		}
	}
	return roomList.at(futherestRoom);
}

float DungeonManager::GetDistance(Room* roomOne, Room* roomTwo)
{
	float vX = roomOne->GetCenter().x - roomTwo->GetCenter().x;
	float vY = roomOne->GetCenter().y - roomTwo->GetCenter().y;
	return static_cast<float>(sqrt((vX * vX) + (vY * vY)));
}




