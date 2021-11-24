#include "Canvas.h"
#include <assert.h> // assert
#include <cstdio> //printf

// Static variables can't be initialized in the class, so now it's here
Tmpl8::Surface* Canvas::screen = nullptr;
Canvas::State Canvas::states[3] = {};
int Canvas::currentState = 0;

void Canvas::Init(Tmpl8::Surface* screenSurface)
{
	screen = screenSurface;

	currentState = 0;

	states[0] = StartState;
	states[1] = RunState;
	states[2] = EndState;
}

void Canvas::Tick()
{
	// Draw UI sprites based on int value (1 -> second array element "1", 15 -> second array element "1" then sixth array element "5", by seperating values)
	states[currentState]();
}

void Canvas::NextState()
{
	// Sanity check if next state exists
	assert((currentState + 1) < 2);
	currentState++;
}

int Canvas::GetCurrentState()
{
	return currentState;
}

void Canvas::StartState()
{

}

void Canvas::RunState()
{

}

void Canvas::EndState()
{

}