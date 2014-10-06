#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include "SheetSprite.h"
#include "Entity.h"
#include "Bullet.h"

using namespace std;

#define MAX_BULLETS 25
#define MAX_ENEMIES 6
#define MAX_LOOT 15

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class PlatformerApp {
public:
	PlatformerApp();
	~PlatformerApp();

	//	Functions for initializing the player and enemy ships for the game
	void Init();

	//	Functions for visual display and for managing the flow of the game
	void Render();
	void Update(float elapsed);
	void FixedUpdate();
	bool UpdateAndRender();

	//	Function for the player to attack the enemy ships with lasers
	void shootBullet();

	void spawnEnemy();

	//	Collision Detection
	bool CollisionCheck(Entity& entity, Bullet& laser);

	//	Function for printing text to the screen
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);

	float lerp(float v0, float v1, float t);

private:
	bool done = false;
	float lastFrameTicks;
	float timeLeftOver = 0.0f;		// Used for fixed update
	SDL_Window* displayWindow;

	//	Textures
	GLuint bg_texture;				// Background texture -- not used for this assignment
	GLuint enemies_texture;			// Enemies texture sheet
	GLuint extras;					// Extra textures
	GLuint font;					// Font texture
	GLuint grass;					// Grass texture sheet
	GLuint items;					// Items texture sheet
	GLuint player_texture;			// Player texture

	//	Vectors
	vector<Entity*> background;		// Vector to hold background (or backgrounds if there's more than one)
	vector<Entity*> entities;		// Vector to hold all entities
	vector<Entity*> loots;			// Vector to hold items that the player can loot

	int score = 0;					// The initial score is 0

	//	Variables for managing player bullet attacks
	int bulletIndex = 0;				// The index of the next laser attack to be fired
	float bullet_cd = 0.0f;			// Cooldown between laser shots
	Bullet bullets[MAX_BULLETS];		// Array for holding all the laser objects that the player will fire

	/*
	int numEnemies;
	*/

	int numLoot = 0;
	float spawnTimer = 0.0f;
	int points = 50;

	//	Variables for gravity
	float gravity_x = 0.0f;
	float gravity_y = 0.8f / 8.0f;
};