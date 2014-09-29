#include "Entity.h"

Entity::~Entity() {}
Entity::Entity(SheetSprite sprite, float x, float y, float scale, float rotation, float speed, float velocity_x, float velocity_y) : sprite(sprite), x(x), y(y), scale(scale), rotation(rotation), speed(speed), velocity_x(velocity_x), velocity_y(velocity_y) {}

void Entity::Render() {
	sprite.Draw(x, y, scale, rotation);
}

void Entity::Update(float elapsed) {
	// Process the movement of the Entity
	if (((velocity_x < 0) || !(x > 1.25)) && ((velocity_x > 0) || !(x < -1.25))){
		x += (velocity_x * elapsed);
	}
}

float Entity::get_x() {
	return x;
}

float Entity::get_y() {
	return y;
}