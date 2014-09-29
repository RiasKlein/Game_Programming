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

/*
//--- Function Prototypes
void RenderMainMenu();
void RenderGameLevel();
void RenderGameOver();
*/

#define MAX_LASERS 25

class SpaceInvadersApp {
public:
	SpaceInvadersApp();
	~SpaceInvadersApp();
	void Init();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);
	void Update_Game(float elapsed);
	bool CollisionCheck(Entity& entity, Laser& laser);
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);
	void shootLaser();
	void ReInit();

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	// Textures
	GLuint bg_textures;			// Backgrounds (and Logo) textures
	GLuint font;				// Font texture
	GLuint fg_textures;			// Foreground textures

	vector<Entity*> backgrounds;
	vector<Entity*> entities;
	vector<Entity*> scene;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state = STATE_MAIN_MENU;
	int score = 0;
	int high_score = 10000;		// Default high score is 10000
	int pts_per_enemy = 5000;

	bool victory = false;		// Determines the content on the game over state

	int laserIndex = 0;
	float laser_cd = 0.0f;
	Laser lasers[MAX_LASERS];
};