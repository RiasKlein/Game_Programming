#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite{
public:
	SheetSprite();

	SheetSprite(GLuint textureID, float u, float v, float width, float height);

	void Draw(float scale_x, float scale_y);
	
	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
};