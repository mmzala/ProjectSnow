#pragma once

#include "surface.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject(char* sprite, Tmpl8::Surface* screen);
	virtual ~GameObject();
	virtual void Tick(float deltaTime);

protected:
	virtual void RenderSprite();

public:
	Transform transform;

protected:
	Tmpl8::Sprite sprite;
	Tmpl8::Surface* screen;
};