#pragma once

class Item;
namespace Tmpl8 {
	class Surface;
}
class ItemManager
{
public:
	ItemManager(char* sprites[3], Tmpl8::Surface* screen);
	~ItemManager();
	bool UseItem(int tileType);
	void SwapItem(int item);

private:
	Item* items[3];
	int holdingItem;
};

