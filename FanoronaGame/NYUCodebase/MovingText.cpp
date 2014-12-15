#include "MovingText.h"

MovingText::MovingText() {}
MovingText::MovingText(SheetSprite sprite, float initial_x, float initial_y, float final_x, float scale, float timeToComplete, bool intermediateStop) :
			sprite(sprite), initial_x(initial_x), initial_y(initial_y), final_x(final_x), scale(scale), timeToComplete(timeToComplete), intermediateStop(intermediateStop) {}

//	This function will update the values of this object
void MovingText::Update(float elapsed){
	// Things should only be happening if we want to animate

	if (animate) {
		//	Is there an intermediate stop and if so did we complete the stop yet?
		if (intermediateStop && !intermediateComplete) {
			// Did we pass the intermediate stop location?
			if (final_x > initial_x) { // We are heading ->
				if (x >= intermediate_x) { // We passed / is at the intermediate spot!
					intermediateTimeOut += elapsed;
				}
			}
			if (final_x < initial_x) { // We are heading <-
				if (x <= intermediate_x) {
					intermediateTimeOut += elapsed;
				}
			}

		}
		else {
			//	Update our position
			if (final_x > initial_x) { // We are heading ->
				float speed = (final_x - initial_x) / timeToComplete;
				x += elapsed * speed;
			}
			else {	// We are heading <-
				float speed = (initial_x - final_x) / timeToComplete;
				x -= elapsed * speed;
			}
		}

		//	Are we out of our intermediate stop yet?
		if (intermediateStop) {
			if (intermediateTimeOut >= intermediateDuration) {
				intermediateComplete = true;
			}
		}
	}

}

//	This function will draw the Moving Text on the screen using SpriteSheet::Draw
void MovingText::Render(){
	sprite.Draw(x, y, scale, rotation, flipped);
}

//	This function resets the animation state of the object
void MovingText::Reset(){
	x = initial_x;
	y = initial_y;
	
	if (intermediateStop) {
		intermediateComplete = false;
	}
	else {
		intermediateComplete = true;
	}

	intermediateTimeOut = 0.0f;
}