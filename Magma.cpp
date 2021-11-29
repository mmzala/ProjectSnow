#include "Magma.h"
#include "MathUtils.h"
#include "Canvas.h"
#include <cmath> // round
#include <cstdio> //printf

Magma::Magma(char* topSprite, char* middleSprite, Tmpl8::Surface* screen)
	: GameObject(middleSprite, screen),
	topSprite(new Tmpl8::Surface(topSprite), 2),
	minSpeed(40.f),
	maxSpeed(90.f),
	currentSpeed(minSpeed),
	animationFrame(0.f),
	animationSpeed(1.f)
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
	float deltaTimeSeconds = deltaTime / 1000;

	// Move the magma only when starting text is gone
	if (Canvas::GetCurrentState() > 0)
	{
		// Move to the top of the screen, make sure it doesn't go outside of it
		transform.position.y -= currentSpeed * deltaTimeSeconds;
		transform.position.y = MathUtils::clip(transform.position.y, minPositionY, maxPositionY);
	}

	Animate(deltaTimeSeconds);
	RenderSprite();
}

// Moves the magma down the given amount
void Magma::MoveDown(float amount)
{
	// Make sure the magma doesn't go beyond the screen
	transform.position.y = MathUtils::clip(transform.position.y + amount, minPositionY, maxPositionY);
}

void Magma::SetSpeed(float speed)
{
	currentSpeed = MathUtils::clip(speed, minSpeed, maxSpeed);
}

void Magma::RenderSprite()
{
	int spritesOnYAxis = (int)((screen->GetHeight() - transform.position.y) / 70 + 1);

	for (int i = 0; i < spritesOnXAxis; i++)
	{
		topSprite.Draw(screen, i * 70, (int)transform.position.y);

		for (int j = 1; j < spritesOnYAxis; j++)
		{
			sprite.Draw(screen, i * 70, (int)transform.position.y + (j * 70));
		}
	}
}

void Magma::Animate(float deltaTimeSeconds)
{
	animationFrame += animationSpeed * deltaTimeSeconds;
	if (animationFrame > topSprite.Frames() - 1) animationFrame = 0.f;
	topSprite.SetFrame((int)std::roundf(animationFrame)); // round off to the nearest frame
}