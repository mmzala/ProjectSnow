#include "Magma.h"
#include "MathUtils.h"
#include "Canvas.h"
#include <cstdio> //printf

Magma::Magma(char* topSprite, char* middleSprite, Tmpl8::Surface* screen)
	: GameObject(topSprite, screen),
	middleSprite(new Tmpl8::Surface(middleSprite), 1),
	speed(50)
{
	spritesOnXAxis = (int)(screen->GetWidth() / 70 + 1);
	minPositionY = 0;
	maxPositionY = (float)(screen->GetHeight() - 60);
	transform.position = Vector2(50.f, maxPositionY);
}

Magma::~Magma()
{}

void Magma::Tick(float deltaTime)
{
	// Move the magma only when starting text is gone
	if (Canvas::GetCurrentState() > 0)
	{
		// Move to the top of the screen, make sure it doesn't go outside of it
		float deltaTimeSeconds = deltaTime / 1000;
		transform.position.y -= speed * deltaTimeSeconds;
		transform.position.y = MathUtils::clip(transform.position.y, minPositionY, maxPositionY);
	}

	RenderSprite();
}

void Magma::RenderSprite()
{
	int spritesOnYAxis = (int)((screen->GetHeight() - transform.position.y) / 70 + 1);

	for (int i = 0; i < spritesOnXAxis; i++)
	{
		sprite.Draw(screen, i * 70, (int)transform.position.y);

		for (int j = 1; j < spritesOnYAxis; j++)
		{
			middleSprite.Draw(screen, i * 70, (int)transform.position.y + (j * 70));
		}
	}
}

// Moves the magma down the given amount
void Magma::MoveDown(float amount)
{
	// Make sure the magma doesn't go beyond the screen
	transform.position.y = MathUtils::clip(transform.position.y + amount, minPositionY, maxPositionY);
}