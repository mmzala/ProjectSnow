#pragma once

#include "Vector.h"
#include "surface.h"
#include <vector>

class Map;
class Surface;
class Player;
class MapManager
{
public:
	MapManager(std::vector<char*> backgroundTiles, std::vector<char*> obstaclesTiles, Player* player, Tmpl8::Surface* screen);
	~MapManager();
	void Tick();
	void AddPositionOffset(Vector2 offset);
	Map* GetMap(int index);

private:
	std::vector<Map*> maps;
	std::vector<char*> backgroundTiles;
	std::vector<char*> obstaclesTiles;
	Player* player;

	Tmpl8::Surface* screen;
};

