#pragma once
#include "raylib.h"
#include <cstdlib>
#include <vector>
#include "config.h"
#include <time.h>
#include "entity.h"
#include "enemy.h"


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
	Config config;
public:
	EntryPoint entries[4];  // 0 Top, 1 Bot, 3 Left, 4 Right
	std::vector<Hallway*> neighborHalls{};
	std::vector<Rectangle> collisionWalls{};

	Room();
	~Room();
	virtual void Render() {};
	virtual void FillContent();
	void Setup(float xPos, float yPos, float l, float s);
	void Activate() { active = true; }
	void InActivate() { active = false; }
	bool GenCollisionCheck(Room* other);
	bool GetIsTaken() { return taken; }
	void SetIsTaken(bool newValue) { taken = newValue; }
	Vector2 GetCenter();
	void CollisionCheck(Entity* entity); 
	void OnCollision(Entity* entity, Rectangle wall); 
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
	void PlaceHallways();
	void FillRooms();
	void AddBonusStuff();
	void AddEnemiesToDungeon(std::vector<Enemy*> enemyList);
	void Render();
	void CollisionCheck(Entity* entity);
	std::vector<Room*> GetRoomList(){return roomList;}
	~DungeonManager();

	void GenSetNeighbor(int index); 
	bool GenGetAvailableEntryPoints(Room* thisRoom, Room* otherRoom, EntryPoint& result);  
	void GenPlaceBossRoom(Room* lastRoom, Room* bossRoom); 
	bool GenListCollisionCheck(int ignoreIndex); 
	Room* FindFutherestRoom(int index);  
	float GetDistance(Room* roomOne, Room* roomTwo); 
};

