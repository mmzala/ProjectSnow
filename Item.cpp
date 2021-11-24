#include "Item.h"

Item::Item(char* sprite, int itemType, Tmpl8::Surface* screen)
	: GameObject(sprite, screen),
	itemType(itemType)
{
	transform.scale = Vector2(1.5f, 1.5f);
}

// TODO: Render item in canvas

// Returns true when using the item was successful
bool Item::Use(int tileType)
{
	// TODO: Make player click multiple times before tile clears

	return tileType == itemType;
}