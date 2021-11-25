#include "ItemManager.h"
#include "Item.h"
#include "Player.h"
#include "Canvas.h"
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

	Canvas::SetItem(items[holdingItem]);
}

ItemManager::~ItemManager()
{
	for (int i = 0; i < 3; i++)
	{
		delete items[i];
	}
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
	Canvas::SetItem(items[holdingItem]);
}