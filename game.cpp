#include "game.h"
#include "surface.h"
#include "MapManager.h"
#include "Map.h"
#include "Player.h"
#include "Magma.h"
#include "Canvas.h"
#include <cstdio> //printf
#include <vector> // vector array

namespace Tmpl8
{
	std::vector<char*> backgroundTiles = { "assets/castleCenter.png" };
	std::vector<char*> obstacleTiles = { "assets/iceWaterDeep.png", "assets/stoneCenter.png" ,"assets/tundraCenter.png" };
	char* items[3] = { "assets/Axe.png", "assets/Pickaxe.png", "assets/Shovel.png" };

	Player* player;
	MapManager* mapManager;
	Magma* magma;

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		mapManager = new MapManager(backgroundTiles, obstacleTiles, screen);
		magma = new Magma("assets/liquidLavaTop_mid.png", "assets/liquidLava.png", screen);
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

	void Game::MouseDown(int button)
	{
		//printf("%i\n", button);
		if (player->IsDead()) return;

		switch (button)
		{
		case 1: // left button
			player->UseItem(mousePosition);
			break;
		case 3: // right button
			break;
		}
	}

	void Game::MouseMove(int x, int y)
	{
		mousePosition = Vector2(x, y);
	}

	void Game::KeyDown(int key)
	{
		//printf("%i\n", key);
		if (player->IsDead())
		{
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