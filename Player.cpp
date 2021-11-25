#include "Player.h"
#include "Map.h"
#include "MathUtils.h"
#include "ItemManager.h"
#include "Canvas.h"
#include <cmath> // round / abs
#include <cstdio> //printf

Player::Player(char* sprite, char* itemSprites[3], Tmpl8::Surface* screen)
	: GameObject(sprite, screen),
	currentMap(nullptr),
	mapPosition(),
	itemManager(new ItemManager(itemSprites, screen))
{}

Player::~Player()
{
	// currentMap pointer will be deleted by MapManager class
	delete itemManager;
}

// Given direction should be relative to the map (map is generated from top to bottom), so up is -1 and down is 1
void Player::Move(Vector2 direction)
{
	// If Canvas is in starting state, then go to next state (this should only happen when player moves for the first time)
	if (Canvas::GetCurrentState() < 1) Canvas::NextState();

	// Make sure the player can't move on full tiles
	if (!currentMap->IsTileClear(mapPosition + direction)) return;

	mapPosition += direction;
	// Clip x coordinate to make sure player doesn't go out of the map
	mapPosition.x = MathUtils::clip(mapPosition.x, 1.f, 10.f);
	transform.position = currentMap->GetTilePosition(mapPosition.x, mapPosition.y);
}

void Player::UseItem(Vector2 mousePosition)
{
	// Get middle position of the player (transform.position is the top left corner of the sprite)
	Vector2 offset(35, 35); // half of a tile
	Vector2 playerPos(transform.position + offset);

	// Get vector from player to mouse position, instead the other way around
	// , so we get reversed values that can be immediately used for map relative position
	Vector2 dir(mousePosition - playerPos);
	dir.Normalize();
	dir = Vector2(std::round(dir.x), std::round(dir.y));

	// Make sure the player can't clear tiles that are in the corners by keeping only the longest part of the vector
	std::abs(dir.x) <= std::abs(dir.y) ? dir.x = 0.f : dir.y = 0.f;

	Vector2 tilePosition(dir + mapPosition);
	int tileType = currentMap->GetTileIndex(tilePosition.x, tilePosition.y);
	if (itemManager->UseItem(tileType))
	{
		currentMap->ClearTile(mapPosition.x + dir.x, mapPosition.y + dir.y);
	}
}

void Player::SwapItem(int item)
{
	itemManager->SwapItem(item);
}

void Player::SwapMap(Map* map)
{
	currentMap = map;
	transform.position = currentMap->GetTilePosition(mapPosition.x, mapPosition.y);
}