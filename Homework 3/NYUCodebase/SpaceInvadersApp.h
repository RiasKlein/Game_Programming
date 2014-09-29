#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include "SheetSprite.h"
#include "Entity.h"
#include "Laser.h"

using namespace std;

#define MAX_LASERS 25

class SpaceInvadersApp {
public:
	SpaceInvadersApp();
	~SpaceInvadersApp();

	//	Functions for initializing the player and enemy ships for the game
	void Init();
	void ReInit();

	//	Functions for visual display and for managing the flow of the game
	void Render();
	void Update(float elapsed);
	void Update_Game(float elapsed);
	bool UpdateAndRender();
	
	//	Function for the player to attack the enemy ships with lasers
	void shootLaser();

	//	Function for determining whether a collision has occured
	bool CollisionCheck(Entity& entity, Laser& laser);

	//	Function for printing text to the screen
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);
	
private:
	bool done = false;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	//	Textures
	GLuint bg_textures;				// Backgrounds (and Logo) textures
	GLuint font;					// Font texture
	GLuint fg_textures;				// Foreground textures

	//	Vectors
	vector<Entity*> backgrounds;	// Vector to hold backgrounds used in various game states
	vector<Entity*> entities;		// Vector to hold the player and alien ships
	vector<Entity*> scene;			// Vector to load in elements required for the game over scenes

	//	The available game states of the game
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state = STATE_MAIN_MENU;	// The initial game state is the main menu

	int score = 0;					// The initial score is 0
	int high_score = 10000;			// Default high score is 10000
	int pts_per_enemy = 5000;		// The amount of points rewarded for the destruction of each enemy ship

	bool victory = false;			// Determines the content on the game over state (whether the player won or not)

	//	Variables for managing player laser attacks
	int laserIndex = 0;				// The index of the next laser attack to be fired
	float laser_cd = 0.0f;			// Cooldown between laser shots
	Laser lasers[MAX_LASERS];		// Array for holding all the laser objects that the player will fire
};