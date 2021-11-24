#pragma once

#include "surface.h"

class Item;
class ItemManager
{
public:
	ItemManager(char* sprites[3], Tmpl8::Surface* screen);
	~ItemManager();
	void Tick(float deltaTime);
	bool UseItem(int tileType);
	void SwapItem(int item);

private:
	Item* items[3];
	int holdingItem;
};

