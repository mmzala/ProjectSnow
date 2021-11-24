#pragma once

#include "GameObject.h"

class Map;
class ItemManager;
class Player : public GameObject
{
public:
	Player(char* sprite, char* itemSprites[3], Tmpl8::Surface* screen);
	~Player();
	void Tick(float deltaTime) override;
	void Move(Vector2 direction);
	void UseItem(Vector2 mousePosition);
	void SwapItem(int item);
	void SwapMap(Map* map);

public:
	// Position on map
	Vector2 mapPosition;

private:
	Map* currentMap;
	ItemManager* itemManager;
};