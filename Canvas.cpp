#include "Canvas.h"
#include "UIText.h"
#include "Vector.h"
#include "Item.h"
#include <SDL_ttf.h>
#include <assert.h> // assert
#include <cstdio> //printf

// Static variables can't be initialized in the class, so now it's here
Canvas::State Canvas::states[3] = {};
int Canvas::currentState = 0;
Tmpl8::Surface* Canvas::screen = nullptr;
SDL_Renderer* Canvas::renderer = nullptr;

Tmpl8::Sprite Canvas::itemBackground = Tmpl8::Sprite(new Tmpl8::Surface("assets/panelInset_blue.png"), 1);
Item* Canvas::item = nullptr;

UIText* Canvas::startText = nullptr;
UIText* Canvas::scoreText = nullptr;

void Canvas::Init(Tmpl8::Surface* screenSurface)
{
	screen = screenSurface;
	currentState = 0;

	states[0] = StartState;
	states[1] = RunState;
	states[2] = EndState;

	// Initialize ttf for text writing
	if (TTF_Init() == -1) {
		printf(TTF_GetError());
	}

	InitStartText();
	InitScoreText();
}

void Canvas::ShutDown()
{
	delete startText;
	delete scoreText;
	TTF_Quit();
}

void Canvas::Tick()
{
	states[currentState]();
}

void Canvas::RenderSprites()
{
	// If canvas is in the last state, don't draw sprites
	if (currentState >= 2) return;

	itemBackground.DrawScaled(5, 5, 80, 80, screen);
	// Item is set on the second frame, so this can't run on the first frame
	if(item) item->Tick(0.f); // Item doesn't need deltaTime
}

void Canvas::NextState()
{
	// Sanity check if next state exists
	assert((currentState + 1) <= 2);
	currentState++;
}

void Canvas::StartState()
{
	startText->Draw();
	scoreText->Draw();
}

void Canvas::RunState()
{
	scoreText->Draw();
}

void Canvas::EndState()
{

}

void Canvas::InitStartText()
{
	Vector2 position;
	position.x = 5;
	position.y = screen->GetHeight() - 25;
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	startText = new UIText(position, "To start the game just start moving...", "assets/KenneyPixel.ttf", 35, color, renderer);
}

void Canvas::InitScoreText()
{
	Vector2 position;
	position.x = 100;
	position.y = 10;
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	scoreText = new UIText(position, "score: 10", "assets/KenneyPixel.ttf", 50, color, renderer);
}

int Canvas::GetCurrentState()
{
	return currentState;
}

void Canvas::SetRenderer(SDL_Renderer* newRenderer)
{
	renderer = newRenderer;
}

void Canvas::SetItem(Item* newItem)
{
	item = newItem;
}