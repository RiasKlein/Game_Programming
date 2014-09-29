#include "Entity.h"

Entity::~Entity() {}
Entity::Entity(SheetSprite sprite, float x, float y, float scale, float rotation, float speed, float velocity_x, float velocity_y) : sprite(sprite), x(x), y(y), scale(scale), rotation(rotation), speed(speed), velocity_x(velocity_x), velocity_y(velocity_y) {}

//	This function will draw the Entity on the screen using SheetSprite::Draw
void Entity::Render() {
	sprite.Draw(x, y, scale, rotation);
}

/*	This function moves the Entity based on its velocity_x (only applicable for the alien ships)
	The player ship moves based on speed rather than the velocity.
*/
void Entity::Update(float elapsed) {
	// Process the movement of the Entity
	if (((velocity_x < 0) || !(x > 1.25)) && ((velocity_x > 0) || !(x < -1.25))){
		x += (velocity_x * elapsed);
	}
}

/*	This function returns the x value of the Entity. 
*/
float Entity::get_x() {
	return x;
}

/*	This function returns the y value of the Entity.
*/
float Entity::get_y() {
	return y;
}