#include "MapManager.h"
#include "Map.h"
#include "Player.h"
#include <algorithm> // copy
#include <cstdio> //printf

MapManager::MapManager(std::vector<char*> backgroundTiles, std::vector<char*> obstaclesTiles, Player* player, Tmpl8::Surface* screen)
	:
	backgroundTiles(backgroundTiles),
	obstaclesTiles(obstaclesTiles),
	player(player),
	screen(screen)
{
	Map* background;
	background = new Map(backgroundTiles, false, screen);
	maps.push_back(background);

	Map* obstacles;
	obstacles = new Map(obstaclesTiles, true, screen);
	maps.push_back(obstacles);
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

Map* MapManager::GetMap(int index)
{
	return maps[index];
}
