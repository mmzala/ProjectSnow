#include "Canvas.h"
#include "UIText.h"
#include "Item.h"
#include <SDL_ttf.h>
#include <assert.h> // assert
#include <cstdio> //printf

// Static variables can't be initialized in the class, so now it's here
Canvas::State Canvas::states[3] = {};
int Canvas::currentState = 0;

Tmpl8::Sprite Canvas::itemBackground = Tmpl8::Sprite(new Tmpl8::Surface("assets/panelInset_blue.png"), 1);
Item* Canvas::item = nullptr;

UIText* Canvas::hintText = nullptr;
UIText* Canvas::scoreText = nullptr;
// Position values are initialized in the init methods
Canvas::TextValues Canvas::hintTextValues[2] = { { "To start the game just start moving..." }, { "You died D: To try again press the \"space\" key!" } };
Canvas::TextValues Canvas::scoreTextValues[2] = { { "SCORE: 0" }, { "SCORE: " } };

Tmpl8::Surface* Canvas::screen = nullptr;
SDL_Renderer* Canvas::renderer = nullptr;

// TODO: center text?

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

	InitHintText();
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

	TransitionState();
}

// Sets the canvas to the start state
void Canvas::Reset()
{
	currentState = 0;
	TransitionState();
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

// Handles state transitions
void Canvas::TransitionState()
{
	switch (currentState)
	{
	case 0: // Start State
		scoreText->SetText(scoreTextValues[0].text);
		scoreText->SetPosition(scoreTextValues[0].position);
		hintText->SetText(hintTextValues[0].text);
		hintText->SetPosition(hintTextValues[0].position);
		break;
	case 2: // End State
		// Show score and hint texts in the middle
		scoreText->SetPosition(Vector2((screen->GetWidth() - scoreText->GetDimensions().x) / 2, scoreTextValues[1].position.y));
		hintText->SetText(hintTextValues[1].text);
		hintText->SetPosition(Vector2((screen->GetWidth() - hintText->GetDimensions().x) / 2, hintTextValues[1].position.y));
		break;
	}
}

void Canvas::InitHintText()
{
	hintTextValues[0].position = Vector2(5, screen->GetHeight() - 25);
	hintTextValues[1].position = Vector2(0, screen->GetHeight() / 2); // First value won't be used
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	hintText = new UIText(hintTextValues[0].position, hintTextValues[0].text, "assets/KenneyPixel.ttf", 35, color, renderer);
}

void Canvas::InitScoreText()
{
	scoreTextValues[0].position = Vector2(100, 10);
	scoreTextValues[1].position = Vector2(0, screen->GetHeight() / 2 - 50); // First value won't be used
	// Black
	SDL_Color color = { 0, 0, 0, 255 };

	// Renderer is set before init is called in template.cpp
	scoreText = new UIText(scoreTextValues[0].position, scoreTextValues[0].text, "assets/KenneyPixel.ttf", 50, color, renderer);
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