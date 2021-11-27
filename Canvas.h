#pragma once

#include "surface.h"
#include <SDL.h>

class UIText;
class Item;
class Canvas
{
public:
	static void Init(Tmpl8::Surface* screenSurface);
	static void ShutDown();
	static void Tick();
	static void RenderSprites();
	static void NextState();
	static int GetCurrentState();
	static void SetScoreText(int amount);
	static void SetRenderer(SDL_Renderer* newRenderer);
	static void SetItem(Item* item);

private:
	// Disallow creating an instance of this object
	Canvas() {}

	static void StartState();
	static void RunState();
	static void EndState();

	// Text initialization
	static void InithintText();
	static void InitScoreText();

private:
	
	// Array of function pointers
	typedef void(*State)();
	static State states[3];
	static int currentState;

	static Tmpl8::Sprite itemBackground;
	static Item* item;

	static UIText* hintText;
	static UIText* scoreText;
	
	static Tmpl8::Surface* screen;
	static SDL_Renderer* renderer;
};

