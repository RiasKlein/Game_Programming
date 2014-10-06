#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include "SheetSprite.h"
#include "Entity.h"

using namespace std;


#define MAX_LOOT 15
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

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

private:
	bool done = false;
	float lastFrameTicks;
	float timeLeftOver = 0.0f;		// Used for fixed update
	SDL_Window* displayWindow;

	//	Textures
	GLuint bg_texture;				// Background texture
	GLuint font;					// Font texture
	GLuint grass;					// Grass texture sheet
	GLuint items;					// Items texture sheet
	GLuint player_texture;			// Player texture

	//	Vectors
	vector<Entity*> background;		// Vector to hold background (or backgrounds if there's more than one)
	vector<Entity*> entities;		// Vector to hold all entities

	//	Variables for Coin Spawning
	int numLoot = 0;
	float spawnTimer = 0.0f;
	int points = 50;

	//	Variables for Gravity
	float gravity_x = 0.0f;
	float gravity_y = 0.8f / 8.0f;
};