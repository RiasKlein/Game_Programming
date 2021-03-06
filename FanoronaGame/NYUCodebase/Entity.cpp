#include "Entity.h"

Entity::Entity() {}
Entity::Entity(SheetSprite sprite, float x, float y, float scale) : sprite(sprite), x(x), y(y), scale(scale) {}

/*	This function updates the Entity
*/
void Entity::Update(float elapsed) {
	//	Update the amount of time we've been on the frame
	timeOnFrame += elapsed;

	//	Look at whether it's time for a frame swap
	if (timeOnFrame > (timeToComplete / (float)animationFrames.size())){
		// It's time for a frame swap
		timeOnFrame = 0.0f;

		// Look at Index to see if it needs to be reset
		frameIndex += 1;
		if (frameIndex == animationFrames.size()) {
			frameIndex = 0;
		}

		sprite = animationFrames[frameIndex];
	}

}

//	This function will draw the Entity on the screen using SheetSprite::Draw
void Entity::Render() {
	sprite.Draw(x, y, scale, rotation, flipped);
}

bool Entity::collidesWith(Entity *entity) {

	// Create variables for the top, bottom, left, and right of the argument objects
	float entity_top = y + (sprite.height / 2.0f);
	float entity_bot = y - (sprite.height / 2.0f);
	float entity_right = x + (sprite.width / 2.0f);
	float entity_left = x - (sprite.width / 2.0f);

	float bullet_top = entity->y + (entity->sprite.height / 2.0f);
	float bullet_bot = entity->y - (entity->sprite.height / 2.0f);
	float bullet_right = entity->x + (entity->sprite.width / 2.0f);
	float bullet_left = entity->x - (entity->sprite.width / 2.0f);

	// Based on the rules for collision detection we have:
	if (entity_bot > bullet_top) {
		return false;
	}
	else if (entity_top < bullet_bot) {
		return false;
	}
	else if (entity_left > bullet_right){
		return false;
	}
	else if (entity_right < bullet_left) {
		return false;
	}

	// A true is returned if there has been a collision
	return true;
}

/*
float Entity::get_x() {
	return x;
}

float Entity::get_y() {
	return y;
}
*/