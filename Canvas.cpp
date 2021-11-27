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

UIText* Canvas::hintText = nullptr;
UIText* Canvas::scoreText = nullptr;

// TODO: center text? and refactor state transitions

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

	InithintText();
	InitScoreText();
}

void Canvas::ShutDown()
{
	delete hintText;
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

	// If transition is to the last state, show scoreand hint in the middle
	if (currentState == 2)
	{
		Vector2 scorePos(screen->GetWidth() / 2 - 75, screen->GetHeight() / 2 - 50);
		scoreText->SetPosition(scorePos);

		Vector2 hintPos(screen->GetWidth() / 2 - 250, screen->GetHeight() / 2);
		hintText->SetText("You died D: To try again press the \"space\" key!");
		hintText->SetPosition(hintPos);
	}
}

void Canvas::StartState()
{
	hintText->Draw();
	scoreText->Draw();
}

void Canvas::RunState()
{
	scoreText->Draw();
}

void Canvas::EndState()
{
	hintText->Draw();
	scoreText->Draw();
}

void Canvas::InithintText()
{
	Vector2 position;
	position.x = 5;
	position.y = screen->GetHeight() - 25;
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	hintText = new UIText(position, "To start the game just start moving...", "assets/KenneyPixel.ttf", 35, color, renderer);
}

void Canvas::InitScoreText()
{
	Vector2 position;
	position.x = 100;
	position.y = 10;
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	scoreText = new UIText(position, "SCORE: 0", "assets/KenneyPixel.ttf", 50, color, renderer);
}

void Canvas::SetScoreText(int score)
{
	char text[32];
	snprintf(text, sizeof(text), "SCORE: %d", score);
	scoreText->SetText(text);
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