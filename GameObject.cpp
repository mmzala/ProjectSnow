#include "GameObject.h"
#include <cstdio> //printf

GameObject::GameObject(char* sprite, Tmpl8::Surface* screen)
	:
	sprite(new Tmpl8::Surface(sprite), 1),
	transform(),
	screen(screen)
{}

GameObject::~GameObject()
{}

void GameObject::Tick(float deltaTime)
{
	RenderSprite();
}

void GameObject::RenderSprite()
{
	Vector2 scale = Vector2();
	scale.x = transform.scale.x * sprite.GetWidth();
	scale.y = transform.scale.y * sprite.GetHeight();

	sprite.DrawScaled(transform.position.x, transform.position.y, scale.x, scale.y, screen);
}