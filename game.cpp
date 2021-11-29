#include "game.h"
#include "surface.h"
#include "MapManager.h"
#include "Map.h"
#include "Player.h"
#include "Magma.h"
#include "Canvas.h"
#include <cstdio> // printf
#include <vector> // vector array

// This game uses:
// SLD2 for all frame buffer interaction / https://www.libsdl.org/
// SLD2_ttf for text rendering / https://www.libsdl.org/projects/SDL_ttf/
// FreeImage to support popular graphics image formats / https://freeimage.sourceforge.io/
// All credits regarding art can be found in the assets folder of this project

// About the increasing process memory:
// While playing the game, you can notice that the process memory increases. I first tought it was
// a memory leak, but after some time debugging with <crtdbg.h> and memory usage snapshots, I didn't find
// anything that could imply that there is one. <crtdbg.h> says there are no memory leaks and the snapshots
// say that the object count that allocate memory does not increase. So there can be a memory leak but
// I am not quite sure myself, maybe I am incorrectly deleting maps from the vector and visual studio is fooling itself.
// It could also be fragmentation (https://en.wikipedia.org/wiki/Fragmentation_(computing))
// , because I am allocating and deallocating chunks of memory often, while the player climbs to the top.
// Anyway, you can play the game normally, and it won't crash that fast because of that problem (at least it never crashed for me).

namespace Tmpl8
{
	std::vector<char*> backgroundTiles = { "assets/castleCenter.png" };
	std::vector<char*> obstacleTiles = { "assets/stoneCenter.png", "assets/iceWaterDeep.png", "assets/tundraCenter.png" };
	char* items[3] = { "assets/Pickaxe.png", "assets/Axe.png", "assets/Shovel.png" };

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		mapManager = new MapManager(backgroundTiles, obstacleTiles, screen);
		magma = new Magma("assets/magma.png", "assets/liquidLava.png", screen);
		player = new Player("assets/p3_stand.png", items, mapManager, magma, screen);
		mapManager->SetPlayer(player);
		player->SetNextMap(mapManager->GetNextObstacleMap());
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		delete player;
		delete magma;
		delete mapManager;
	}

	// -----------------------------------------------------------
	// Resets application to starting state
	// -----------------------------------------------------------
	void Game::Reset()
	{
		Shutdown();
		Init();
		Canvas::Reset();
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		// clear the graphics window
		screen->Clear(0);
		
		mapManager->Tick();
		player->Tick(deltaTime);
		magma->Tick(deltaTime);

		//screen->Line(player->transform.position.x + 35, player->transform.position.y + 35, mousePosition.x, mousePosition.y, 0xffffff);
	}

	void Game::MouseDown(int button, int mousePosX, int mousePosY)
	{
		//printf("%i\n", button);
		if (player->IsDead()) return;

		switch (button)
		{
		case 1: // left button
			player->UseItem(Vector2(mousePosX, mousePosY));
			break;
		case 3: // right button
			break;
		}
	}

	void Game::KeyDown(int key)
	{
		//printf("%i\n", key);
		if (player->IsDead())
		{
			if (key == 44) // Spacebar
			{
				Reset();
			}

			return;
		}

		switch (key)
		{
		// Movement
		case 26: // W
			player->Move(Vector2(0, -1));
			break;
		case 4: // A
			player->Move(Vector2(-1, 0));
			break;
		case 22: // S
			player->Move(Vector2(0, 1));
			break;
		case 7: // D
			player->Move(Vector2(1, 0));
			break;

		// Item swapping
		case 30: // 1
			player->SwapItem(0); // Axe
			break;
		case 31: // 2
			player->SwapItem(1); // Pickaxe
			break;
		case 32: // 3
			player->SwapItem(2); // Shovel
			break;
		}
	}
};