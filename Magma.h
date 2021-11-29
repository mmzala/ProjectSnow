#pragma once

#include "GameObject.h"

class Magma : public GameObject
{
public:
	Magma(char* topSprite, char* middleSprite, Tmpl8::Surface* screen);
	~Magma();
	void Tick(float deltaTime) override;
	void MoveDown(float amount);

protected:
	void RenderSprite() override;
	void Animate(float deltaTimeSeconds);

private:
	Tmpl8::Sprite topSprite;
	float speed;
	// Determines how many sprites there are needed for the X axis
	int spritesOnXAxis;
	// Positions that the magma has to stay between
	float minPositionY;
	float maxPositionY;
	//Animation variables
	float animationFrame;
	float animationSpeed;
};

