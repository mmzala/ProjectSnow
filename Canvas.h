#pragma once

#include "surface.h"

class Canvas
{
public:
	static void Init(Tmpl8::Surface* screenSurface);
	static void Tick();
	static void NextState();
	static int GetCurrentState();

private:
	// Disallow creating an instance of this object
	Canvas() {}

	static void StartState();
	static void RunState();
	static void EndState();

private:
	static Tmpl8::Surface* screen;

	// Array of function pointers
	typedef void(*State)();
	static State states[3];
	static int currentState;
};

