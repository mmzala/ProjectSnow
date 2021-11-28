#pragma once

#include "surface.h"
#include "Vector.h"
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
	static void Reset();
	static int GetCurrentState();
	static void SetScoreText(int amount);
	static void SetRenderer(SDL_Renderer* newRenderer);
	static void SetItem(Item* item);

public:
	struct TextValues
	{
		char* text;
		Vector2 position;

		TextValues(char* text)
		{
			this->text = text;
			this->position = Vector2();
		}
	};

private:
	// Disallow creating an instance of this object
	Canvas() {}
	static void StartState();
	static void RunState();
	static void EndState();
	static void TransitionState();
	// Text initialization
	static void InitHintText();
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
	// These 2 TextValues hold the values needed for the StartState and EndState
	static TextValues hintTextValues[2];
	static TextValues scoreTextValues[2];
	
	static Tmpl8::Surface* screen;
	static SDL_Renderer* renderer;
};

