#pragma once

#include "Vector.h"
#include <vector>

class Map;
class Player;
namespace Tmpl8 {
	class Surface;
}
class MapManager
{
public:
	MapManager(std::vector<char*> backgroundTiles, std::vector<char*> obstaclesTiles, Tmpl8::Surface* screen);
	~MapManager();
	void Tick();
	void MoveMaps(float y);
	void AddPositionOffset(Vector2 offset);
	Map* GetMap(int index);
	Map* GetNextObstacleMap();
	void SetPlayer(Player* player);

private:
	void CreateNextMaps();
	void DestroyPreviousMaps();

private:
	std::vector<Map*> maps;
	std::vector<char*> backgroundTiles;
	std::vector<char*> obstaclesTiles;
	Player* player;
	Tmpl8::Surface* screen;
};

