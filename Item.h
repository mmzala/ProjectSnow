#pragma once

#include "GameObject.h"

class Item : public GameObject
{
public:
	Item(char* sprite, int itemType, Tmpl8::Surface* screen);
	bool Use(int tileType);

public:
	const int itemType;
};