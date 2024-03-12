#pragma once
#include "raylib.h"
#include "room.h"
#include <cstdlib>
#include <vector>
#include "config.h"
#include <time.h>
#include "entity.h"
#include "enemy.h"




class DungeonManager
{
private:
	Config config;
	std::vector<Room*> roomList={};
	std::vector<Hallway*> hallwayList = {};
	float longSide = 300.f*config.PIXEL_SCALE;
	float shortSide = 200.f*config.PIXEL_SCALE;
	int roomAmount = 9;

	Texture2D hallVertTxr{};
	Texture2D hallHoriTxr{};
	
	Texture2D spawnTxr{};
	Texture2D bossTxr{};
	Texture2D roomHoriTxr{};
	Texture2D roomVertTxr{};
public:
	void GenerateDungeon();
	void LoadTextures();
	void CreateRooms();
	void PlaceHallways();
	void FillRooms();
	void AddBonusStuff();
	void AddEnemiesToDungeon(std::vector<Enemy*> enemyList);
	void Render(Rectangle cam);
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

