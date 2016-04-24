#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include "ParticleEmitter.h"

#define FIXED_TIMESTEP	0.0166666f
#define MAX_TIMESTEPS	6
#define MAX_BULLETS		30
#define BULLET_COOLDOWN	0.25f

using namespace std;

class AsteroidsApp{
public:
	AsteroidsApp();
	~AsteroidsApp();

	void Init();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
	bool UpdateAndRender();
	bool checkCollision(Entity* entity1, Entity* entity2);
	void shootBullet();

private:
	bool done;
	float lastFrameTicks = 0.0f;
	float timeLeftOver = 0.0f;
	SDL_Window* displayWindow;

	float gravity_x = 0.0f;
	float gravity_y = 0.0f;

	GLuint spriteSheetTexture;
	SheetSprite bulletSprite;
	Player* player;
	vector<Entity*> entities;
	Entity* background;

	int bulletIndex = 0;
	Bullet bullets[MAX_BULLETS];
	float bulletCD = 0.0f;

	Mix_Chunk* bullet;
	Mix_Music* music;

	vector<ParticleEmitter> particleEmitters;
};
