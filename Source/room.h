#pragma once
#include "raylib.h"
#include "config.h"
#include <vector>
#include "entity.h"

class Room;

enum class SIDE
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct EntryPoint
{
	SIDE side = SIDE::UP;
	Vector2 position{ 0.f, 0.f };
	bool isAvailable = true;
};

class Hallway : Rectangle
{
private:
	Room* owners[2] = { nullptr };
	Config config;
	SIDE side = SIDE::UP;
	Texture2D txr{};
	//Vector2 originPoint{ 0.f, 0.f };
public:
	std::vector<Rectangle> collisionWalls{};
	void Setup(EntryPoint src, Room* ownerOne, Room* ownerTwo, Texture2D newTxr);
	void Render(Rectangle cam);
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
	Texture2D txr{};
public:
	Color color = GRAY;
	EntryPoint entries[4];  // 0 Top, 1 Bot, 3 Left, 4 Right
	std::vector<Hallway*> neighborHalls{};
	std::vector<Rectangle> collisionWalls{};

	Room();
	~Room();
	virtual void Render(Rectangle cam);
	virtual void FillContent();
	void Setup(float xPos, float yPos, Texture2D newTxr);
	void Activate() { active = true; }
	void InActivate() { active = false; }
	bool GenCollisionCheck(Room* other);
	bool GetIsTaken() { return taken; }
	void SetIsTaken(bool newValue) { taken = newValue; }
	Vector2 GetCenter();
	void CollisionCheck(Entity* entity);
	void OnCollision(Entity* entity, Rectangle wall);
	int GetCorrectHallway(SIDE ownerOneSide, SIDE ownerTwoSide);
	void CreateCollisionWallSingle(EntryPoint entry, float thickness);
	void CreateCollisionWallMulti(EntryPoint entry, float thickness, int hallwayIndex);
	void CreateCollisionWalls(EntryPoint entry, float thickness, SIDE ownerOneSide, SIDE ownerTwoSide);
};
class SpawnRoom : public Room
{
private:
public:
	void Render(Rectangle cam) override
	{
		color = WHITE;
		Room::Render(cam);
	}
};
class BossRoom : public Room
{
private:
public:
	void Render(Rectangle cam) override
	{
		color = VIOLET;
		Room::Render(cam);
	}
};
class EnemyRoom : public Room
{
private:

public:
	void Render(Rectangle cam) override
	{
		Room::Render(cam);
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