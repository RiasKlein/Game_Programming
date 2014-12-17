#pragma once
#include "SheetSprite.h"
#include <cmath>
#include <math.h>

class MovingText {
public:
	MovingText();
	MovingText(SheetSprite sprite, float initial_x, float initial_y, float final_x, float scale, float timeToComplete);

	SheetSprite sprite;

	void Update(float elapsed);
	void Render();
	void Reset();

	float initial_x = 0.0f;					// The initial position x
	float initial_y = 0.0f;					// The initial position y
	float final_x = 0.0f;					// The final position x
	float timeToComplete = 0.0f;			// How long do we have to get from initial position to the final position?
	
	bool animate = false;					// Should this object start moving?

	bool arrived = false;
	float displayTime = 0.0f;				// How long to display the object for auto reset (VALUE MUST BE GIVEN)
	float timeUpSoFar = 0.0f;				// How long the object has been displaying itself

	//	Variables required for SheetSprite::Draw
	float x = 3.0f;
	float y = 3.0f;
	float scale = 4.0f;
	float rotation = 0.0f;
	bool flipped = false;
	
private:
};