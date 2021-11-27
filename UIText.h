#pragma once

#include "Vector.h"
#include <SDL_ttf.h>

class UIText
{
public:
	UIText(Vector2 position, char* text, char* font, int fontSize, SDL_Color color, SDL_Renderer* renderer);
	~UIText();
	void Draw();
	void SetText(char* text);
	void CreateText();
	void SetPosition(Vector2 position);

private:
	TTF_Font* font;
	char* text;
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect position;
};

