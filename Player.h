#pragma once

#include "GameObject.h"

class Map;
class MapManager;
class ItemManager;
class Player : public GameObject
{
public:
	Player(char* sprite, char* itemSprites[3], MapManager* mapManager, Tmpl8::Surface* screen);
	~Player();
	void Move(Vector2 direction);
	void UseItem(Vector2 mousePosition);
	void SwapItem(int item);
	void CalculatePosition();
	void SetNextMap(Map* map);
	Map* GetCurrentMap();
	Map* GetPreviousMap();

public:
	// Position on map
	Vector2 mapPosition;

private:
	bool TransitionMapsUp();
	bool TransitionMapsDown();
	Map* GetMapBasedOnDir(Vector2 direction);
	int GetTileIndexOfMap(Map* map, Vector2& position);

private:
	MapManager* mapManager;
	Map* previousMap;
	Map* currentMap;
	Map* nextMap;
	ItemManager* itemManager;
};