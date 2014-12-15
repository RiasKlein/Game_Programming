#pragma once
#include "SheetSprite.h"
#include <cmath>
#include <math.h>

class MovingText {
public:
	MovingText();
	MovingText(SheetSprite sprite, float initial_x, float initial_y, float final_x, float scale, float timeToComplete, bool intermediateStop);

	SheetSprite sprite;

	void Update(float elapsed);
	void Render();
	void Reset();

	float initial_x = 0.0f;					// The initial position x
	float initial_y = 0.0f;					// The initial position y
	float final_x = 0.0f;					// The final position x
	float timeToComplete = 0.0f;			// How long do we have to get from initial position to the final position?
	bool intermediateStop = false;			// Does this object stop somewhere on the way to the final destination?

	bool animate = false;					// Should this object start moving?

	//	Variables required for SheetSprite::Draw
	float x = 3.0f;
	float y = 3.0f;
	float scale = 1.0f;
	float rotation = 0.0f;
	bool flipped = false;

	//	Intermediate Stop?
	bool intermediateComplete = false;		// Did we finish the intermediate part?

	//	The following parameters must be GIVEN
	float intermediate_x = 0.0f;			// X position of location to stop at
	float intermediateDuration = 0.0f;		// How long to stay at the intermediate position (A negative number would stay there forever)
	float intermediateTimeOut = 0.0f;		// When should the intermediate stop

	
private:
};