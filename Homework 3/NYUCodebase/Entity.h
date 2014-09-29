#pragma once

#include "SheetSprite.h"

class Entity {
	//This version of Entity is not the same as the one used for PONG -- since a lot of the fields for the PONG Entity has been incorporated into SheetSprite
public:
	~Entity();
	Entity(SheetSprite sprite, float x, float y, float scale, float rotation, float speed, float velocity_x, float velocity_y);
	
	SheetSprite sprite;

	// Properties required for SheetSprite::Draw
	float x = 0.0f;
	float y = 0.0f;
	float scale = 1.0f;
	float rotation = 0.0f;

	float speed = 0.0f;
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;

	void Render();
	void Update(float elapsed);

	// Functions to get the X / Y in order to process movement and gameover
	float get_x();
	float get_y();
};