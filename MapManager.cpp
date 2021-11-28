#include "MapManager.h"
#include "Map.h"
#include "Player.h"
#include <algorithm> // copy
#include <cstdio> //printf

MapManager::MapManager(std::vector<char*> backgroundTiles, std::vector<char*> obstaclesTiles, Tmpl8::Surface* screen)
	:
	backgroundTiles(backgroundTiles),
	obstaclesTiles(obstaclesTiles),
	player(nullptr), // Player is set after the player is created in game.cpp
	screen(screen)
{
	// Initialize first maps
	Map* background;
	background = new Map(backgroundTiles, false, 0.f, screen);
	maps.push_back(background);

	Map* obstacles;
	obstacles = new Map(obstaclesTiles, true, 0.f, screen);
	maps.push_back(obstacles);

	// Create next maps in advance
	CreateNextMaps();
}

MapManager::~MapManager()
{
	for (int i = 0; i < maps.size(); i++)
	{
		delete maps[i];
	}
	maps.clear();
}

void MapManager::Tick()
{
	// Draw all current maps
	for (int i = 0; i <= maps.size() - 1; i++)
	{
		maps[i]->DrawMap();
	}
}

void MapManager::AddPositionOffset(Vector2 offset)
{
	for (int i = 0; i <= maps.size() - 1; i++)
	{
		maps[i]->positionOffset += offset;
	}
}

void MapManager::MoveMaps(float y)
{
	// Scroll the maps
	AddPositionOffset(Vector2(0.f, y));

	// If the current map is the last created map, create new ones
	if (player->GetCurrentMap() == maps.back())
	{
		CreateNextMaps();
	}
}

Map* MapManager::GetMap(int index)
{
	return maps[index];
}

Map* MapManager::GetNextObstacleMap()
{
	// Next obstacle map is always the last map in the vector, because of the order that the maps are created in
	return maps.back();
}

void MapManager::SetPlayer(Player* player)
{
	this->player = player;
}

void MapManager::CreateNextMaps()
{
	// 70 is the width and height of the tile sprites
	float posOff = maps.back()->positionOffset.y + (maps.back()->sizeY * 70);

	Map* background;
	background = new Map(backgroundTiles, false, posOff, screen);
	maps.push_back(background);

	Map* obstacles;
	obstacles = new Map(obstaclesTiles, true, posOff, screen);
	maps.push_back(obstacles);

	// Make sure player exists
	if (player)
	{
		player->SetNextMap(obstacles);
		DestroyPreviousMaps();
	}

	
}

void MapManager::DestroyPreviousMaps()
{
	// If player needs the first maps as his previous map, then return
	if (player->GetPreviousMap() == maps[1]) return;

	// Delete first 2 maps in the vector (background and obstacle maps)
	delete maps[0];
	maps.erase(maps.begin());
	delete maps[0];
	maps.erase(maps.begin());

	// After erasing, we need to shrink the vector, so it's not going to get bigger and bigger
	maps.shrink_to_fit();

	// Use this to debug
	//printf("%zi\n", maps.capacity());
	//printf("%zi\n", maps.size());
}
