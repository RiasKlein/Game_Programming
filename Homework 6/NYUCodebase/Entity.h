#pragma once
#include "SheetSprite.h"
#define FIXED_TIMESTEP 0.0333333f // 30 FPS

/*	The Entity class will be used for all the objects 
*/
class Entity {
public:
	Entity();
	Entity(SheetSprite sprite, float x, float y, float scale, float velocity_x, float velocity_y);

	SheetSprite sprite;

	//	Functions
	void Update();
	void Render();
	bool collidesWith(Entity *entity);
	void FixedUpdate();
	void Jump();

	//	Functions to get the X / Y coordinates (carried over from asgn3)
	float get_x();
	float get_y();

	//	Variables required for SheetSprite::Draw
	float x = 0.0f;
	float y = 0.0f;
	float scale = 1.0f;
	float rotation = 0.0f;
	bool flipped = false;				

	//	Variables affecting Entity movement
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;
	float friction_x = 10.0f;
	float friction_y = 0.0f;
	
	float mass;							// Will not be used for this assignment

	//	Variables affecting Collision
	bool isStatic = false;				// Should forces affect this entity? We shall assume false (for most entities) and set to true when needed 

	//	Collsion Detection
	bool collidedTop;					// A collision has been detected with the top of this entity
	bool collidedBot;					// Collision with bottom
	bool collidedRight;					// Collision with right 
	bool collidedLeft;					// Collision with left

	//	Flags 
	bool isPlayer = false;				// Default entities are NOT the player
	bool isEnemy = false;				// Default entities are NOT enemies
	bool inJump = false;				// Whether the entity is jumping 
	bool playerDead = false;			// Player dies if hit by enemy (left or right contact)
	bool enemyDead = false;
	bool isLoot = false;
	bool looted = false;

	//	Score Keeping
	int score = 0;
};