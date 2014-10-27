#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

/*	The SheetSprite class is used to manage textures for objects.
*/
class SheetSprite {
public:
	SheetSprite();
	SheetSprite(GLuint textureID, unsigned int Xsprite, unsigned int Ysprite, unsigned int index);

	void Draw(float x, float y, float scale, float rotation);

	GLuint textureID;

	unsigned int Xsprite;
	unsigned int Ysprite;
	unsigned int index;

	float width;
	float height;
};
