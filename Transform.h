#pragma once

#include "Vector.h"

class Transform
{
public:
	Transform()
		:
		position(),
		scale(1, 1)
	{}

public:
	Vector2 position;
	Vector2 scale;
};