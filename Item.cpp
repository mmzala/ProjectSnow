#include "Item.h"

Item::Item(char* sprite, int itemType, Tmpl8::Surface* screen)
	: GameObject(sprite, screen),
	itemType(itemType)
{
	// Set transform values for UI
	transform.position = Vector2(10.f, 10.f);
	transform.scale = Vector2(2.f, 2.f);
}

// Returns true when using the item was successful
bool Item::Use(int tileType)
{
	return tileType == itemType;
}