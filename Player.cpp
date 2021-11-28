#include "Player.h"
#include "Map.h"
#include "MapManager.h"
#include "MathUtils.h"
#include "ItemManager.h"
#include "Magma.h"
#include "Canvas.h"
#include <cmath> // round / abs
#include <cstdio> // printf

Player::Player(char* sprite, char* itemSprites[3], MapManager* mapManager, Magma* magma, Tmpl8::Surface* screen)
	: GameObject(sprite, screen),
	mapManager(mapManager),
	previousMap(nullptr),
	currentMap(mapManager->GetMap(1)),
	nextMap(nullptr),
	mapPosition(mapManager->GetMap(1)->FindHoleFromEnd(Vector2(4, 2), Vector2(2, 2))),
	itemManager(new ItemManager(itemSprites, screen)),
	magma(magma),
	positionOffset(10, 5),
	scrollTreshhold(150),
	isDead(false),
	points(0)
{
	transform.scale = Vector2(0.7f, 0.7f);
	// Calculate starting position
	CalculatePosition();
}

Player::~Player()
{
	delete itemManager;
}

void Player::Tick(float deltaTime)
{
	// Check if player touched magma
	if (magma->transform.position.y < transform.position.y)
	{
		isDead = true;

		// If canvas isn't in the last state, then go to the last state
		if(Canvas::GetCurrentState() < 2)Canvas::NextState();
	}

	RenderSprite();
}

// Given direction should be relative to the map (map is generated from top to bottom), so up is -1 and down is 1
void Player::Move(Vector2 direction)
{
	// If Canvas is in starting state, then go to next state (this should only happen when player moves/uses item for the first time)
	if (Canvas::GetCurrentState() < 1) Canvas::NextState();

	if(!CanMove(direction)) return;
	mapPosition += direction;
	// Clip x coordinate to make sure player doesn't go out of the map
	mapPosition.x = MathUtils::clip(mapPosition.x, 1.f, 10.f);

	CheckForScrolling();
	CalculatePosition();
	AddPoints((int)-direction.y);
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

	if (itemManager->UseItem(tileType))
	{
		// If Canvas is in starting state, then go to next state (this should only happen when player moves/uses item for the first time)
		if (Canvas::GetCurrentState() < 1) Canvas::NextState();

		map->ClearTile((int)tilePosition.x, (int)tilePosition.y);
		AddPoints(tileType);
	}
}

void Player::SwapItem(int item)
{
	itemManager->SwapItem(item);
}

void Player::SetNextMap(Map* map)
{
	nextMap = map;
}

void Player::CalculatePosition()
{
	Vector2 tilePosition = currentMap->GetTilePosition((int)mapPosition.x, (int)mapPosition.y);
	transform.position = tilePosition + positionOffset;
}

// Checks if the player can move to the next tile, returns false if not
bool Player::CanMove(Vector2 direction)
{
	// If standing at the top or bottom of currentMap, try to transition maps
	if (mapPosition.y == 0 && direction.y == -1) // Up
	{
		if (!TransitionMapsUp()) return false;
	}
	else if (mapPosition.y == currentMap->sizeY - 1 && direction.y == 1) // Down
	{
		if (!TransitionMapsDown()) return false;
	}
	else
	{
		// Make sure the player can't move on full tiles (this is already done in transition methods)
		if (!currentMap->IsTileClear(mapPosition + direction)) return false;
	}

	return true;
}

// Returns false when can't transition
bool Player::TransitionMapsUp()
{
	Vector2 nextPosition(mapPosition.x, (float)nextMap->sizeY - 1);
	if (!nextMap->IsTileClear(nextPosition)) return false;

	previousMap = currentMap;
	currentMap = nextMap;
	nextMap = mapManager->GetNextObstacleMap();

	// Set current mapPosition relative to next map
	nextPosition.y += 1;
	mapPosition = nextPosition;

	//printf("trans next map\n");
	return true;
}

// Returns false when can't transition
bool Player::TransitionMapsDown()
{
	Vector2 nextPosition(mapPosition.x, 0.f);
	if (!previousMap->IsTileClear(nextPosition)) return false;

	nextMap = currentMap;
	currentMap = previousMap;
	previousMap = nullptr;

	// Set current mapPosition relative to next map
	nextPosition.y -= 1;
	mapPosition = nextPosition;

	//printf("trans previous map\n");
	return true;
}

void Player::CheckForScrolling()
{
	Vector2 tilePosition = currentMap->GetTilePosition((int)mapPosition.x, (int)mapPosition.y);
	// If player is high enough scroll the maps and move the magma back
	if (tilePosition.y < scrollTreshhold)
	{
		mapManager->MoveMaps(-70);
		magma->MoveDown(70);
	}
}

void Player::AddPoints(int amount)
{
	//  If points don't change, just return to not change text
	if (amount == 0) return;

	points += amount;
	Canvas::SetScoreText(points);
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
		position = Vector2(position.x, (float)nextMap->sizeY - 1);
		return nextMap->GetTileIndex((int)position.x, (int)position.y);
	}
	else if (map == previousMap)
	{
		position = Vector2(position.x, 0.f);
		return previousMap->GetTileIndex((int)position.x, (int)position.y);
	}

	return currentMap->GetTileIndex((int)position.x, (int)position.y);
}

Map* Player::GetCurrentMap()
{
	return currentMap;
}

Map* Player::GetPreviousMap()
{
	return previousMap;
}

bool Player::IsDead()
{
	return isDead;
}