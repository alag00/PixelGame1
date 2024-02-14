#pragma once
#include "raylib.h"
#include <cstdlib>
#include <vector>
#include "config.h"
#include <time.h>
#include "entity.h"
#include "enemy.h"


/*
	TODO LIST:
	DungeonManager should have some of the room methods
*/
class Room;
enum class SIDE
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct EntryPoint
{
	SIDE side = SIDE::UP;
	Vector2 position{0.f, 0.f};
	bool isAvailable = true;
};

class Hallway : Rectangle
{
private:
	Room* owners[2] = {nullptr};
	Config config;
	SIDE side = SIDE::UP;
public:
	std::vector<Rectangle> collisionWalls{};
	void Setup(EntryPoint src, Room* ownerOne, Room* ownerTwo, float bonusLen);
	void Render();
	void FillContent();
	void CollisionCheck(Entity* entity); // check if touching entity
	void OnCollision(Entity* entity, Rectangle wall); // push entity back 
	float GetX() { return x; }
	float GetY() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	Room* GetOwnerOne() { return owners[0]; }
	Room* GetOwnerTwo() { return owners[1]; }
	SIDE GetSide() { return side; }
};
class Room : public Rectangle
{
private:
	bool taken = false;
	bool active = false;
	EntryPoint entries[4];  // 0 Top, 1 Bot, 3 Left, 4 Right
	Config config;
public:
	std::vector<Hallway*> neighborHalls{};
	std::vector<Rectangle> collisionWalls{};

	Room();
	~Room();
	virtual void Render(){};
	virtual void FillContent();
	void Setup(float xPos, float yPos, float l, float s);
	void Activate() { active = true; }
	void InActivate() { active = false; }
	 bool GenCollisionCheck(Room* other);	
	 void GenSetNeighbor(std::vector<Room*> &list, int index, std::vector<Hallway*>& hallList);
	 bool GenGetAvailableEntryPoints(Room* other, EntryPoint& result);
	 void GenPlaceBossRoom(Room* bossRoom, std::vector<Hallway*>& hallList);
	 bool GetIsTaken() { return taken; }
	 void SetIsTaken(bool newValue) { taken = newValue; }
	bool GenListCollisionCheck(std::vector<Room*>& list, int ignoreIndex);
	Room* FindFutherestRoom(std::vector<Room*>& list);
	float GetDistance(Room* room);
	Vector2 GetCenter();
	void CollisionCheck(Entity* entity); // check if touching entity
	void OnCollision(Entity* entity, Rectangle wall); // push entity back 
	int GetCorrectHallway(SIDE ownerOneSide, SIDE ownerTwoSide);
};
class SpawnRoom : public Room
{
private:
public:
	void Render() override
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), WHITE);
		for (int i = 0; i < collisionWalls.size(); i++)
		{
			DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), YELLOW);
		}
	}
};
class BossRoom : public Room
{
private:
public:
	void Render() override
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), VIOLET);
		for (int i = 0; i < collisionWalls.size(); i++)
		{
			DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), YELLOW);
		}
	}
};
class EnemyRoom : public Room
{
private:
	Color color = WHITE;
public:
	void Render() override
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);

		for (int i = 0; i < collisionWalls.size(); i++)
		{
			DrawRectangle(static_cast<int>(collisionWalls.at(i).x), static_cast<int>(collisionWalls.at(i).y), static_cast<int>(collisionWalls.at(i).width), static_cast<int>(collisionWalls.at(i).height), YELLOW);
		}

	}
	void SetRandomColor()
	{
		color.a = 127;
		color.r = rand() % 127 + 1;
		color.g = rand() % 127 + 1;
		color.b = rand() % 127 + 1;
	}
};
class DeityRoom : public Room
{
private:
public:
};




class DungeonManager
{
private:
	Config config;
	std::vector<Room*> roomList={};
	std::vector<Hallway*> hallwayList = {};
	float longSide = 300.f*config.PIXEL_SCALE;
	float shortSide = 200.f*config.PIXEL_SCALE;
	int roomAmount = 9;

public:
	void GenerateDungeon();
	void CreateRooms();
	void FixRoomPosition();
	void PlaceHallways();
	void FillRooms();
	void AddBonusStuff();
	void AddEnemiesToDungeon(std::vector<Enemy*> enemyList);
	void Render();
	void CollisionCheck(Entity* entity);
	std::vector<Room*> GetRoomList(){return roomList;}
	~DungeonManager();

	//void GenSetNeighbor(int index);
	//bool GenGetAvailableEntryPoints(Room* other, EntryPoint& result);
};




//	NEW
	// Right after hallway setup, check if the room overlaps another Taken Room
//
/// <summary>
	// Entities entering a room will add them to this room's entity list and remove them from their previous rooms entityList
	// UpdateEntities will only update activate rooms which is the one the Player is in and connected rooms
/// </summary>
/*
class Room : public Rectangle
{
private:
	Color color = WHITE;
	Color typeColor = PINK; // RED = BOSS, GRAY = NORMAL, BLACK = DEITY, WHITE = SPAWN
	static Config config;
	Room* neighborRooms = nullptr; //[config.MAX_ROOM_NEIGHBOR_COUNT] {nullptr};
	short drawMargin = 10;
public:
	//short int customLimit = 1;// config.MAX_ROOM_NEIGHBOR_COUNT;
	void Setup(float a_x, float a_y, float a_w, float a_h, Color c)
	{

		x = a_x;
		y = a_y;
		width = a_w;
		height = a_h;
		color = c;
	}
	void SplitRoomHorizontal(std::vector<Room*> &roomList)
	{
		int randNum = rand() % 3 + 1;
		float divider = 2.f;
		switch (randNum)
		{
		case 1:
			divider = 1.75f;
			break;
		case 2:
			divider = 2.f;
			break;
		case 3:
			divider = 2.25f;
			break;
		}

		float newHeight = height / divider;
		float oldWidth = width;
		float oldX = x;
		float oldY = y;

		Room* botRoom = new Room();
		botRoom->Setup(x, y + newHeight, width, newHeight, GetRandomColor());
		roomList.push_back(botRoom);

		x = oldX;
		y = oldY;
		width = oldWidth;
		height = newHeight;
		color = GetRandomColor();
	}
	void SplitRoomVertical(std::vector<Room*> &roomList)
	{

		int randNum = rand() % 3 + 1;
		float divider = 2.f;
		switch (randNum)
		{
		case 1:
			divider = 1.75f;
			break;
		case 2:
			divider = 2.f;
			break;
		case 3:
			divider = 2.25f;
			break;
		}

		float newWidth = width / divider;
		float oldHeight = height;
		float oldX = x;
		float oldY = y;

		Room* rightRoom = new Room();
		rightRoom->Setup(x + newWidth, y, newWidth, height, GetRandomColor());
		roomList.push_back(rightRoom);

		x = oldX;
		y = oldY;
		height = oldHeight;
		width = newWidth;
		color = GetRandomColor();
	}

	void Render()
	{
		DrawRectangle(static_cast<int>(x + drawMargin), static_cast<int>(y + drawMargin), static_cast<int>(width - drawMargin), static_cast<int>(height - drawMargin), color);
		DrawCircle(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), 5.f, typeColor);


		DrawLine(static_cast<int>(GetCenter().x), static_cast<int>(GetCenter().y), static_cast<int>(neighborRooms->GetCenter().x), static_cast<int>(neighborRooms->GetCenter().y), WHITE);


	}
	Color GetRandomColor()
	{
		Color randColor = WHITE;
		randColor.a = 127;
		randColor.r = rand() % 127 + 1;
		randColor.g = rand() % 127 + 1;
		randColor.b = rand() % 127 + 1;

		return randColor;
	}
	bool CollisionCheck(Room* room)
	{
		if (x < room->x + room->width && x + width > room->x &&
			y < room->y + room->height && y + height > room->y)
		{
			int randNum = rand() % 10 + 1;
			if (x < room->x)
			{
				x -= randNum;
			}
			else
			{
				x += randNum;
			}
			if (y < room->y)
			{
				y -= randNum;
			}
			else
			{
				y += randNum;
			}
			return true;
		}
		return false;
	}
	float GetArea()
	{
		return width * height;
	}
	Vector2 GetCenter()
	{
		return { (x + (width / 2.f)), (y + (height / 2.f)) };
	}
	void SetRoomType(Color c)
	{
		typeColor = c;
	}
	void UpdateEntities()
	{

	}
	void RenderEntities()
	{

	}
	float GetDistance(Room* room)
	{
		float vX = GetCenter().x - room->GetCenter().x;
		float vY = GetCenter().y - room->GetCenter().y;
		return sqrt((vX * vX) + (vY * vY));
	}
	void SetNeighbors(std::vector<Room*>& roomList, int ignoreIndex)
	{

		short int closestRoom = 0;
		for (short int j = 1; j < roomList.size(); j++)
		{
			if (j == ignoreIndex)
			{
				continue;
			}
			if (GetDistance(roomList.at(j)) < GetDistance(roomList.at(closestRoom)))
			{
				closestRoom = j;
			}
		}
		neighborRooms = roomList.at(closestRoom);
	}

};

class RoomManager {
private:
	int totalRooms = 0;
	std::vector<Room*> roomList;
	Room* activeRoom = nullptr;
	float mapPosX = 100.f;
	float mapPosY = 100.f;
	float mapWidth = 1000.f;
	float mapHeight = 500.f;
public:
	void Setup()
	{
		CreateRooms();
		FixRoomPositions();
		ConnectAndLabelRooms();
		FillRoomContent();
	}
	void CreateRooms()
	{
		totalRooms = 15;//rand() % 15 + 6;
		Room* firstRoom = new Room();
		firstRoom->Setup(mapPosX, mapPosY, mapWidth, mapHeight, WHITE);
		roomList.push_back(firstRoom);
		while (roomList.size() < totalRooms)
		{
			int currentRoomCount = static_cast<int>(roomList.size());
			for (int i = 0; i < currentRoomCount; i++)
			{
				if (roomList.size() >= totalRooms)
				{
					break;
				}
				roomList.at(i)->SplitRoomVertical(roomList);
			}

			currentRoomCount = static_cast<int>(roomList.size());
			for (int i = 0; i < currentRoomCount; i++)
			{
				if (roomList.size() >= totalRooms)
				{
					break;
				}
				roomList.at(i)->SplitRoomHorizontal(roomList);
			}
		}
	}
	void FixRoomPositions()
	{
		// while non of the rooms are touching eachother
		bool isFixingPos = true;
		while (isFixingPos)
		{
			isFixingPos = false;
			for (int i = 0; i < roomList.size(); i++)
			{
				for (int j = 0; j < roomList.size(); j++)
				{
					if (i != j && roomList.at(i)->CollisionCheck(roomList.at(j)))
					{
						isFixingPos = true;
						// push rooms from each other
					}
				}
			}
		}

	}

	float GetDistance(Room* room1, Room* room2)
	{
		float vX = room1->GetCenter().x - room2->GetCenter().x;
		float vY = room1->GetCenter().y - room2->GetCenter().y;
		return sqrt((vX * vX) + (vY * vY));
	}
	void ConnectAndLabelRooms()
	{
		int bossIndex = 0;
		for (int i = 1; i < roomList.size(); i++)
		{
			if (roomList.at(i)->GetArea() > roomList.at(bossIndex)->GetArea())
			{
				bossIndex = i;
			}
		}
		roomList.at(bossIndex)->SetRoomType(RED);

		//roomList.at(bossIndex) == BossRoom;
		int spawnIndex = 0;
		float currentLen = 0.f;
		for (int i = 0; i < roomList.size(); i++)
		{
			float length = GetDistance(roomList.at(bossIndex), roomList.at(i));
			if (length > currentLen)
			{
				currentLen = length;
				spawnIndex = i;
			}
		}
		roomList.at(spawnIndex)->SetRoomType(WHITE);

		//roomList.at(spawnIndex) == SpawnRoom;

		int deityChance = rand() % 100 + 1;
		if (deityChance <= 3)
		{
			// 2 room
		}
		else if (deityChance <= 30)
		{
			// 1 room
		}

		for (int i = 0; i < roomList.size(); i++)
		{
			if (i == bossIndex || i == spawnIndex)
			{
				continue;
			}
			roomList.at(i)->SetRoomType(DARKGRAY);
		}

		for (int i = 0; i < roomList.size(); i++)
		{
			roomList.at(i)->SetNeighbors(roomList, i);
		}

		// Closest room becomes neighbors

		// Create non-diagonal lines

	}
	void FillRoomContent()
	{
		// Add StaticEntities
		// Walls and interior
	}
	void Render()
	{
		if (roomList.size() <= 0)
		{
			return;
		}
		for (int i = 0; i < roomList.size(); i++)
		{
			roomList.at(i)->Render();
		}
	}
	~RoomManager()
	{
		for (int i = 0; i < roomList.size(); i++)
		{
			delete roomList.at(i);
			roomList.at(i) = nullptr;
		}
	}
};

*/
