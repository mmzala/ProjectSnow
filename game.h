#pragma once

#include "Vector.h"

class MapManager;
class Magma;
class Player;
namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Reset();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button, int mousePosX, int mousePosY);
	void MouseMove(int x, int y) { /* implement if you want to handle keys */ };
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown(int key);

private:
	Surface* screen;
	Player* player;
	MapManager* mapManager;
	Magma* magma;
};

}; // namespace Tmpl8