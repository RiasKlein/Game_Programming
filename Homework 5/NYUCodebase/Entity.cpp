#include "Entity.h"

Entity::Entity() {}
Entity::Entity(SheetSprite sprite, float x, float y, float scale, float velocity_x, float velocity_y) : sprite(sprite), x(x), y(y), scale(scale), velocity_x(velocity_x), velocity_y(velocity_y) {}

/*	This function moves the Entity based on its velocity_x
*/
void Entity::Update() {
	x += velocity_x * FIXED_TIMESTEP;
	y += velocity_y * FIXED_TIMESTEP;
}

//	This function will draw the Entity on the screen using SheetSprite::Draw
void Entity::Render() {
	sprite.Draw(x, y, scale, rotation);
}

void Entity::Jump() {
	inJump = true;
	velocity_y = 52.0f * FIXED_TIMESTEP;
	collidedBot = false;
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

