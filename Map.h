#pragma once

#include "surface.h"
#include "Vector.h"
#include <vector>

/* This does only work with pointers?
// Forward decleration using namspaces
namespace Tmpl8 {
	class Surface;
	class Sprite;
}
*/
class Map
{
public:
	Map(std::vector<char*> spriteFiles, bool shouldIncludeHoles, float posOffsetY, Tmpl8::Surface* screen);
	~Map();
	void DrawMap();
	Vector2 GetTilePosition(int row, int column);
	int GetTileIndex(int row, int column);
	Vector2 FindHoleFromEnd();
	Vector2 FindHoleFromEnd(int skipRows, int skipColumns);
	bool IsTileClear(Vector2 mapPosition);
	void ClearTile(int row, int column);

private:
	void GenerateMap();

public:
	Vector2 positionOffset;

	// All maps will have those dimensions
	static const int sizeX = 12;
	static const int sizeY = 8;

private:
	int map[sizeX][sizeY];
	bool shouldIncludeHoles;

	std::vector<Tmpl8::Sprite*> tiles;
	Tmpl8::Surface* screen;
};