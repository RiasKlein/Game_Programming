#pragma once
#include "SheetSprite.h"
#include <vector>

class Entity {
public:
	Entity();
	Entity(SheetSprite sprite, float x, float y, float scale);

	SheetSprite sprite;							// Default sprite for this entity

	//	Functions
	void Update(float elapsed);
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

	//	Animation requirements
	float timeToComplete = 0.0f;				//	Total time we have to complete the animations	(MUST BE GIVEN)
	float timeOnFrame = 0.0f;					//	Time spent on the current frame
	int frameIndex = 0;							//	Starting frame index is 0
	std::vector<SheetSprite> animationFrames;	//	Vector of animation frames						(MUST BE GIVEN)

	//	Variables affecting Collision (NOT USED IN THIS PROJECT)
	bool isStatic = false;						// Should forces affect this entity? We shall assume false (for most entities) and set to true when needed 

	//	Collsion Detection (NOT USED IN THIS PROJECT)
	bool collidedTop;							// A collision has been detected with the top of this entity
	bool collidedBot;							// Collision with bottom
	bool collidedRight;							// Collision with right 
	bool collidedLeft;							// Collision with left
};