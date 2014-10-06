#pragma once
#include "SheetSprite.h"

/*	This class is used for all the Bullet attacks that the player fires.
*/
class Bullet{
public:
	Bullet();
	Bullet(SheetSprite sprite, float x, float y, float scale, float rotation, float speed);

	SheetSprite sprite;

	// Variables required for SheetSprite::Draw (to render the Bullet objects on the screen)
	float x;
	float y;
	float scale = 1.0f;
	float rotation = 0.0f;

	float speed = 2.0f;		// Determines how fast the Bullet will travel on the screen
	float visible = true;	// Determines whether the Bullet is capable of destroying an enemy craft

	void Update(float elapsed);
	void Render();
};