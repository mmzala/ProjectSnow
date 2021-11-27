#pragma once

#include "GameObject.h"

class Map;
class MapManager;
class ItemManager;
class Magma;
class Player : public GameObject
{
public:
	Player(char* sprite, char* itemSprites[3], MapManager* mapManager, Magma* magma, Tmpl8::Surface* screen);
	~Player();
	void Tick(float deltaTime) override;
	void Move(Vector2 direction);
	void UseItem(Vector2 mousePosition);
	void SwapItem(int item);
	void CalculatePosition();
	void SetNextMap(Map* map);
	Map* GetCurrentMap();
	Map* GetPreviousMap();
	bool IsDead();

public:
	// Position on map
	Vector2 mapPosition;

private:
	bool TransitionMapsUp();
	bool TransitionMapsDown();
	void CheckForScrolling();
	void AddPoints(int amount);
	Map* GetMapBasedOnDir(Vector2 direction);
	int GetTileIndexOfMap(Map* map, Vector2& position);

private:
	ItemManager* itemManager;
	MapManager* mapManager;
	Map* previousMap;
	Map* currentMap;
	Map* nextMap;
	Magma* magma;

	// Determines when to move the maps and magma
	float scrollTreshhold;
	bool isDead;
	int points;
};