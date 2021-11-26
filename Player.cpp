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
	// currentMap pointer will be deleted by MapManager class
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
	if (mapPosition.y == currentMap->sizeY - 1 && direction.y == 1)
	{
		if (!TransitionMapsDown()) return;
	}

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
	nextPosition.y += 1;
	mapPosition = nextPosition;

	printf("next");
}

bool Player::TransitionMapsDown()
{
	Vector2 nextPosition(mapPosition.x, 0);
	if (!previousMap->IsTileClear(nextPosition)) return false;

	nextMap = currentMap;
	currentMap = previousMap;
	nextPosition.y -= 1;
	mapPosition = nextPosition;

	printf("previous");
}