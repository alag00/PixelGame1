#pragma once
struct Config
{
	short int screenWidth = 1600;
	short int screenHeight = 900;
	short int fpsTarget = 60;

	short int maxRoomCount = 20;
	short int minRoomCount = 4;
	const static short int MAX_ROOM_NEIGHBOR_COUNT = 5;
	const float PIXEL_SCALE = 4.f;
	const float HALLWAY_LENGTH = 100.f * PIXEL_SCALE;
};