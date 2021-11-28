#pragma once

#include <math.h> // sqrt

class Vector2
{
public:
	float x;
	float y;

	Vector2()
		:
		x(0.f),
		y(0.f)
	{}

	Vector2(float x, float y)
		:
		x(x),
		y(y)
	{}

	Vector2(int x, int y)
		:
		x((float)x),
		y((float)y)
	{}

	float Magnitude()
	{
		return sqrtf(this->x * this->x + this->y * this->y);
	}

	void Normalize()
	{
		float magnitude = this->Magnitude();

		this->x = this->x / magnitude;
		this->y = this->y / magnitude;
	}

	// Operator overloads
	Vector2 operator+=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
};