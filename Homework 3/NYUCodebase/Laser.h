#pragma once

#include "SheetSprite.h"

class Laser{
public:
	Laser();
	Laser(SheetSprite sprite, float x, float y, float scale, float rotation, float speed);

	SheetSprite sprite;

	float x;
	float y;
	float scale = 1.0f;
	float rotation = 0.0f;

	float speed = 2.0f;
	float visible = true;

	void Update(float elapsed);
	void Render();
};