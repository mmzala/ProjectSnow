#include "UIText.h"
#include <SDL.h>
#include <cstdio> //printf

UIText::UIText(Vector2 position, char* text, char* font, int fontSize, SDL_Color color, SDL_Renderer* renderer)
	:
	text(text),
	font(TTF_OpenFont(font, fontSize)),
	color(color),
	renderer(renderer)
{
	this->position.x = position.x;
	this->position.y = position.y;
	CreateText();
}

UIText::~UIText()
{
	TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
}

void UIText::Draw()
{
	SDL_RenderCopy(renderer, texture, nullptr, &position);
}

void UIText::SetText(char* text)
{
	this->text = text;
	CreateText();
}

void UIText::CreateText()
{
	// Make sure the old texture is cleaned up
	if (!texture) SDL_DestroyTexture(texture);

	SDL_Surface* surface = TTF_RenderText_Solid(this->font, this->text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_QueryTexture(texture, nullptr, nullptr, &this->position.w, &this->position.h);
}

void UIText::SetPosition(Vector2 position)
{
	this->position.x = position.x;
	this->position.y = position.y;
}