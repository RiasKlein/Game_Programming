#include "Laser.h"

Laser::Laser() {
	// Spawn attacks outside the player's FOV
	// Objects between 1.33 and 1.00 can be seen
	x = 1.5f;
	y = 1.5f;
}

Laser::Laser(SheetSprite sprite, float x, float y, float scale, float rotation, float speed) : sprite(sprite), x(x), y(y), scale(scale), rotation(rotation), speed(speed) {}

/*	This function will change the laser's y location on the map based on the speed of the laser.
*/
void Laser::Update(float elapsed){
	y += speed * elapsed;
}

/*	This function will draw lasers on the screen if they are visible.
	Invisible lasers should not be capable of doing damage to enemy ships.
*/
void Laser::Render(){
	if (visible){
		sprite.Draw(x, y, scale, rotation);
	}
}