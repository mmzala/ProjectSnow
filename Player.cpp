#include "Player.h"
#include "Map.h"
#include "MathUtils.h"
#include "ItemManager.h"
#include "Canvas.h"
#include <cmath> // round / abs
#include <cstdio> //printf

Player::Player(char* sprite, char* itemSprites[3], Map* startingMap, Tmpl8::Surface* screen)
	: GameObject(sprite, screen),
	previousMap(nullptr),
	currentMap(startingMap),
	nextMap(nullptr),
	mapPosition(startingMap->FindHoleFromEnd(5, 3)),
	itemManager(new ItemManager(itemSprites, screen))
{
	transform.scale = Vector2(0.7, 0.7);
}

Player::~Player()
{
	// Map pointers will be deleted by MapManager class
	delete itemManager;
}

// Given direction should be relative to the map (map is generated from top to bottom), so up is -1 and down is 1
void Player::Move(Vector2 direction)
{
	// If Canvas is in starting state, then go to next state (this should only happen when player moves for the first time)
	if (Canvas::GetCurrentState() < 1) Canvas::NextState();

	// If standing at the top or bottom of currentMap, try to transition maps
	if (mapPosition.y == 0 && direction.y == -1)
	{
		if (!TransitionMapsUp()) return;
	}
	else if (mapPosition.y == currentMap->sizeY - 1 && direction.y == 1)
	{
		if (!TransitionMapsDown()) return;
	}
	else
	{
		// Make sure the player can't move on full tiles (this is already done in transition methods)
		if (!currentMap->IsTileClear(mapPosition + direction)) return;
	}
	
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

	// Check what map the player want's to break a tile in
	Vector2 tilePosition(dir + mapPosition);
	Map* map = GetMapBasedOnDir(dir);
	int tileType = GetTileIndexOfMap(map, tilePosition);

	printf("%i", tileType);
	if (itemManager->UseItem(tileType))
	{
		map->ClearTile(tilePosition.x, tilePosition.y);
	}
}

void Player::RecalculatePosition()
{
	transform.position = currentMap->GetTilePosition(mapPosition.x, mapPosition.y);
}

void Player::SwapItem(int item)
{
	itemManager->SwapItem(item);
}

void Player::SetNextMap(Map* map)
{
	nextMap = map;
}

bool Player::TransitionMapsUp()
{
	Vector2 nextPosition(mapPosition.x, nextMap->sizeY - 1);
	if (!nextMap->IsTileClear(nextPosition)) return false;

	previousMap = currentMap;
	currentMap = nextMap;
	nextMap = nullptr;

	// Set current mapPosition relative to next map
	nextPosition.y += 1;
	mapPosition = nextPosition;

	printf("trans next map\n");
}

bool Player::TransitionMapsDown()
{
	Vector2 nextPosition(mapPosition.x, 0);
	if (!previousMap->IsTileClear(nextPosition)) return false;

	nextMap = currentMap;
	currentMap = previousMap;
	previousMap = nullptr;

	// Set current mapPosition relative to next map
	nextPosition.y -= 1;
	mapPosition = nextPosition;

	printf("trans previous map\n");
}

// Returns map based on given direction and mapPosition
Map* Player::GetMapBasedOnDir(Vector2 direction)
{
	// Up
	if (mapPosition.y == 0 && direction.y == -1)
	{
		return nextMap;
	}
	// Down
	else if (mapPosition.y == currentMap->sizeY - 1 && direction.y == 1)
	{
		return previousMap;
	}
	
	return currentMap;
}

// Gets tile index inside a specific map and updates map position so it's relative to the used map
int Player::GetTileIndexOfMap(Map* map, Vector2& position)
{
	// Can't use siwtch with pointers, casting to intptr_t could work, but this only
	// works assuming sizeof(int) == sizeof(ptr) on the target platform, that's not always the case
	if (map == nextMap)
	{
		position = Vector2(position.x, nextMap->sizeY - 1);
		return nextMap->GetTileIndex((int)position.x, (int)position.y);
	}
	else if (map == previousMap)
	{
		position = Vector2(position.x, 0);
		return previousMap->GetTileIndex((int)position.x, (int)position.y);
	}

	return currentMap->GetTileIndex((int)position.x, (int)position.y);
}