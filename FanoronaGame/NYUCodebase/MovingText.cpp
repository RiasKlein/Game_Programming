#include "MovingText.h"

MovingText::MovingText() {}
MovingText::MovingText(SheetSprite sprite, float initial_x, float initial_y, float final_x, float scale, float timeToComplete) :
			sprite(sprite), initial_x(initial_x), initial_y(initial_y), final_x(final_x), scale(scale), timeToComplete(timeToComplete) {}

//	This function will update the values of this object
void MovingText::Update(float elapsed){
	// Things should only be happening if we want to animate

	if (animate) {
		float speed = (final_x - initial_x) / timeToComplete;

		if (final_x >= initial_x) { // Heading ->
			if (x < final_x) {
				x += speed * elapsed;
			}
			else {
				arrived = true;

				if (displayTime > 0.0f) {
					timeUpSoFar += elapsed;
				}
			}

		}

		if (final_x <= initial_x) { // Heading <-
			if (x > final_x) {
				x += speed * elapsed;
			}
			else {
				arrived = true;

				if (displayTime > 0.0f) {
					timeUpSoFar += elapsed;
				}
			}
		}

		if (displayTime > 0.0f) {
			if (timeUpSoFar >= displayTime) {
				Reset();
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

	animate = false;
	arrived = false;
	timeUpSoFar = 0.0f;
}