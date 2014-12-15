#pragma once
#include "SheetSprite.h"
#define FIXED_TIMESTEP 0.0333333f // 30 FPS

/*	The Entity class will be used for all the objects 
*/
class Entity {
public:
	Entity();
	Entity(SheetSprite sprite, float x, float y, float scale);

	SheetSprite sprite;			// Default sprite for this entity

	//	Functions
	void Update();
	void Render();
	bool collidesWith(Entity *entity);
	void FixedUpdate();

	//	Functions to get the X / Y coordinates (carried over from asgn3)
	float get_x();
	float get_y();

	//	Variables required for SheetSprite::Draw
	float x = 0.0f;
	float y = 0.0f;
	float scale = 1.0f;
	float rotation = 0.0f;
	bool flipped = false;				

	//	Variables affecting Collision
	bool isStatic = false;				// Should forces affect this entity? We shall assume false (for most entities) and set to true when needed 

	//	Collsion Detection
	bool collidedTop;					// A collision has been detected with the top of this entity
	bool collidedBot;					// Collision with bottom
	bool collidedRight;					// Collision with right 
	bool collidedLeft;					// Collision with left
};