#pragma once
#include "SheetSprite.h"

/*	The Entity class will be used for all the objects in this game except for laser attacks.
Included objects are ships (player and alien), backgrounds, logos, bases, special effects.
*/
class Entity {
	// This version of Entity is not the same as the one used for PONG, since a lot of the fields for the PONG Entity has been incorporated into SheetSprite
public:
	Entity();
	Entity(SheetSprite sprite, float x, float y, float scale, float velocity_x, float velocity_y);

	SheetSprite sprite;

	// Functions
	void Update(float elapsed);
	void Render();

	bool collidesWith(Entity *entity);
	void FixedUpdate();
	void Jump();

	// Functions to get the X / Y coordinates in order to process movement and whether the game is over
	float get_x();
	float get_y();

	// Variables required for SheetSprite::Draw
	float x = 0.0f;
	float y = 0.0f;
	float scale = 1.0f;
	float rotation = 0.0f;
	bool flipped = false;				// whether we should flip the drawing of the object

	// Variables affecting Entity movement
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;
	float friction_x = 0.4f;
	float friction_y = 0.0f;
	
	float mass; // Will not be used for this assignment

	// Variables affecting Collision
	bool isStatic = false;				// Should forces affect this entity? We shall assume false (for most entities) and set to true when needed 

	//bool enableCollisions = true;		// Can this entity be touched by others? We shall default to assuming true (and set false when needed)

	bool collidedTop;					// A collision has been detected with the top of this entity
	bool collidedBot;					// Collision with bottom
	bool collidedRight;					// Collision with right 
	bool collidedLeft;					// Collision with left

	bool isPlayer = false;				// Default entities are NOT the player
	bool isEnemy = false;				// Default entities are NOT enemies
	bool inJump = false;				// Whether the entity is jumping 
	bool playerDead = false;			// Player dies if hit by enemy (left or right contact)
	bool enemyDead = false;
	bool isLoot = false;
	bool looted = false;

	int score = 0;
};