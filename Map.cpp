#include "Map.h"
#include "surface.h"
#include <stdlib.h> // rand
#include <time.h> // time (for randomizing rand)
#include <cstdio> //printf

Map::Map(std::vector<char*> spriteFiles, bool shouldIncludeHoles, float posOffsetY, Tmpl8::Surface* screen)
	:
	// Calculate positionOffset, so that the map fills the whole window equally (tiles are 70x70)
	positionOffset((float)(sizeX * 70 - screen->GetWidth()) / 2, posOffsetY),
	shouldIncludeHoles(shouldIncludeHoles),
	screen(screen)
{
	// Create all sprites and push it into the vector
	tiles.reserve(spriteFiles.size());
	for  (int i = 0; i < spriteFiles.size(); i++)
	{
		tiles.emplace_back(new Tmpl8::Sprite(new Tmpl8::Surface(spriteFiles[i]), 1));
	}

	GenerateMap();
}

Map::~Map()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		delete tiles[i];
	}
	tiles.clear();
}

void Map::DrawMap()
{
	for (int row = 0; row <= sizeX - 1; row++)
	{
		for (int column = 0; column <= sizeY - 1; column++)
		{
			// If there is no index available, that means the map should have holes, so skip to the next index 
			if (map[row][column] > tiles.size() - 1) continue;

			tiles[map[row][column]]->Draw(screen, row * 70 - (int)positionOffset.x, column * 70 - (int)positionOffset.y);
		}
	}
}

/// <summary>
/// Finds closest hole in the map from the end and returns it's position in the map array
/// </summary>
/// <param name="skipRows"> Specifies how many rows to skip from left and right </param>
/// <param name="skipColumns"> Specifies how many columns to skip from left and right </param>
/// <returns></returns>
Vector2 Map::FindHoleFromEnd(Vector2 skipRows, Vector2 skipColumns)
{
	for (int column = sizeY - (int)skipColumns.y - 1; column >= skipColumns.x; column--)
	{
		for (int row = sizeX - (int)skipRows.y - 1; row >= skipRows.x; row--)
		{
			if (map[row][column] >= tiles.size()) return Vector2(row, column);
		}
	}

	// If the map should have holes, but none are found, just make a hole in the center and return that
	if (shouldIncludeHoles)
	{
		Vector2 center(sizeX / 2, sizeY / 2);
		map[(int)center.x][(int)center.y] = (int)tiles.size();
		return center;
	}

	return Vector2();
}

Vector2 Map::GetTilePosition(int row, int column)
{
	Vector2 position = Vector2();
	position.x = row * 70 - positionOffset.x;
	position.y = column * 70 - positionOffset.y;

	return position;
}

// Returns what sort of tile it is
int Map::GetTileIndex(int row, int column)
{
	return map[row][column];
}

// Returns true when tile is empty (doesn't have any sprite)
bool Map::IsTileClear(Vector2 mapPosition)
{
	return map[(int)mapPosition.x][(int)mapPosition.y] == tiles.size();
}

void Map::ClearTile(int row, int column)
{
	map[row][column] = (int)tiles.size();
}

void Map::GenerateMap()
{
	//printf("\nGenerated map:\n");

	// Seeding rand with time to get random numbers
	srand((unsigned int)time(NULL));

	for (int row = 0; row <= sizeX - 1; row++)
	{
		for (int column = 0; column <= sizeY - 1; column++)
		{
			// Generate random index between 0 and the available number of tile sprites, if there should be sholes
			// in the map, generate between 0 and the available number of tile sprites + 1
			map[row][column] = rand() % (tiles.size() + shouldIncludeHoles);

			//printf("%d", map[row][column]);
		}
		//printf("\n");
	}
}