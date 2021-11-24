#include "ItemManager.h"
#include "Item.h"
#include "Player.h"
#include <assert.h> // assert

ItemManager::ItemManager(char* sprites[3], Tmpl8::Surface* screen)
	:
	items(),
	holdingItem(0)
{
	for (int i = 0 ; i < 3; i++)
	{
		items[i] = new Item(sprites[i], i, screen);
	}
}

ItemManager::~ItemManager()
{
	for (int i = 0; i < 3; i++)
	{
		delete items[i];
	}
}

void ItemManager::Tick(float deltaTime)
{
	items[holdingItem]->Tick(deltaTime);
}

bool ItemManager::UseItem(int tileType)
{
	return items[holdingItem]->Use(tileType);
}

void ItemManager::SwapItem(int item)
{
	// There are only 3 items
	assert(item <= 3);
	holdingItem = item;
}