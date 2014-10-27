#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include "SheetSprite.h"
#include "Entity.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;


#define MAX_LOOT 15
#define FIXED_TIMESTEP 0.0333333f // 30 FPS
#define MAX_TIMESTEPS 6
#define SPRITE_COUNT_X	39
#define SPRITE_COUNT_Y	1
#define TILE_SIZE 0.15f

class PlatformerApp {
public:
	PlatformerApp();
	~PlatformerApp();

	//	Function for setting up players and walls
	void Init();

	//	Functions for visual display and for managing the flow of the game
	void Render();
	void Update(float elapsed);
	void FixedUpdate();
	bool UpdateAndRender();

	//	Function for printing text to the screen
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);

	//	Lerp Function
	float lerp(float v0, float v1, float t);

	//	New Functions
	void buildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	void RenderLevel();

	void worldToTileCoordinates(float x, float y, int &gridX, int &gridY);
	void tileToWorldCoordinatesX(float &x, int gridX);
	void tileToWorldCoordinatesY(float &y, int gridY);

	bool collisionx();
	bool collisiony();

private:
	bool done = false;
	float lastFrameTicks = 0.0f;
	float timeLeftOver = 0.0f;		// Used for fixed update
	SDL_Window* displayWindow;

	//	Textures
	GLuint spritesheet;				// Sprite sheet 
	GLuint bgTexture;				// Background texture
	GLuint font;

	//	Vectors
	vector<Entity*> background;		// Vector to hold background (or backgrounds if there's more than one)
	vector<Entity*> entities;		// Vector to hold all entities

	// Map
	unsigned int mapWidth;
	unsigned int mapHeight;
	unsigned char** levelData;

	//	Variables for Gravity
	float gravity_x = 0.0f;
	float gravity_y = 2.0f;
};