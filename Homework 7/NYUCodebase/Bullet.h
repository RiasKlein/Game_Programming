#pragma once
#include "Entity.h"
#include "SheetSprite.h"

class Bullet : public Entity {
public:
	Bullet();

	void Update(float elapsed);
	void Render();

	bool visible;
};