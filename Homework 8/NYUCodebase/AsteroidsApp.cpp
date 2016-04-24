#include "AsteroidsApp.h"

//	LERP and Random Number Generation functions
float genRandomNum(float low, float high) {
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SDL_FreeSurface(surface);

	return textureID;
}

AsteroidsApp::AsteroidsApp() {
	Init();
	srand(time(NULL));
	done = false;

	Mix_PlayMusic(music, -1);
}

AsteroidsApp::~AsteroidsApp() {
	// Cleanup and quit
	SDL_Quit();
}

void AsteroidsApp::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse - Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	//	Load in Textures & Generate Entities
	spriteSheetTexture = LoadTexture("sprites.png");
	bulletSprite = SheetSprite(spriteSheetTexture, 841.0f / 1024.0f, 647.0f / 1024.0f, 13.0f / 1024.0f, 37.0f / 1024.0f);

	//	Player entity
	SheetSprite playerSprite = SheetSprite(spriteSheetTexture, 120.0f / 1024.0f, 604.0f / 1024.0f, 104.0f / 1024.0f, 84.0f / 1024.0f);
	player = new Player();
	player->sprite = playerSprite;
	player->x = 0.0f;
	player->y = 0.0f;
	player->scale_x = 0.75f;
	player->scale_y = 0.75f;
	player->friction_x = 1.0f;
	player->friction_y = 1.0f;
	entities.push_back(player);

	//	Asteroid entities
	SheetSprite asteroidSprite = SheetSprite(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
	for (int i = 0; i < 20; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidSprite;
		asteroid->x = genRandomNum(-1.33f, 1.33f);
		asteroid->y = genRandomNum(-1.0f, 1.0f);
		asteroid->rotation = genRandomNum(-90.0f, 90.0f);

		float scale = genRandomNum(0.85f, 1.45f);
		asteroid->scale_x = scale;
		asteroid->scale_y = scale;

		asteroid->velocity_x = genRandomNum(-0.25f, 0.25f);
		asteroid->velocity_y = genRandomNum(-0.25f, 0.25f);
		entities.push_back(asteroid);
	}

	//	Background entity
	GLuint background_image = LoadTexture("background.png");
	SheetSprite background_sprite = SheetSprite(background_image, 0.0f, 0.0f, 1024.0f, 1024.0f);
	background = new Entity();
	background->sprite = background_sprite;
	background->x = -1.33f;
	background->y = -1.0f;
	background->scale_x = 1.5f;
	background->scale_y = 1.5f;

	//	Load in Music and Sounds
	bullet = Mix_LoadWAV("bullet.wav");

	//	Load in the music and set the appropriate volume
	music = Mix_LoadMUS("music.mp3");
	Mix_VolumeMusic(50);
}

void AsteroidsApp::Update(float elapsed) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	//	Shooting
	if (keys[SDL_SCANCODE_SPACE]) {
		if (bulletCD > BULLET_COOLDOWN){
			shootBullet();
		}
	}

	//	Movement
	if (keys[SDL_SCANCODE_UP]) {
		player->velocity_x = 0.8f;
		player->velocity_y = 0.8f;

		ParticleEmitter engineParticles(50);
		engineParticles.maxLifetime = 0.1f;
		engineParticles.position = Vector(player->x, player->y, 0.0f);
		engineParticles.velocity = Vector(-sin(player->rotation)*player->velocity_x*2, cos(player->rotation)*player->velocity_y*2, 0.0f);
		engineParticles.velocityDeviation = Vector(0.3f, 0.5f, 0.0f);
		particleEmitters.push_back(engineParticles);
		particleEmitters.back().effect();
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		player->velocity_x = 0.0f;
		player->velocity_y = 0.0f;
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		player->rotation += 3.0f * elapsed;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		player->rotation -= 3.0f * elapsed;
	}

	//	Update Entities / Bullets / Particles
	for (unsigned int i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (unsigned int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	for (unsigned int i = 0; i < particleEmitters.size(); i++){
		particleEmitters[i].Update(elapsed);
	}

	//	Limiting the player's position
	if (player->x < -1.28f) {
		player->x = -1.28f;
	}
	else if (player->x > 1.28f) {
		player->x = 1.28f;
	}
	if (player->y < -0.95f) {
		player->y = -0.95f;
	}
	else if (player->y > 0.95f) {
		player->y = 0.95f;
	}

	//	Update bullet cooldown
	bulletCD += elapsed;

	//	Clearing Particles
	for (unsigned int i = 0; i < particleEmitters.size(); i++){
		if (particleEmitters[i].lifetime > particleEmitters[i].maxLifetime) {
			particleEmitters[i].particles.clear();

		}
	}
}

void AsteroidsApp::FixedUpdate() {

	for (unsigned int i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();
		
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (checkCollision(entities[i], entities[j])) {

					Vector distance = Vector(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
					float distLength = distance.length();
					distance.normalize();

					entities[i]->x += distance.x * 0.0001f / (distLength * distLength);
					entities[i]->y += distance.y * 0.0001f / (distLength * distLength);

					entities[j]->x -= distance.x * 0.0001f / (distLength * distLength);
					entities[j]->y -= distance.y * 0.0001f / (distLength * distLength);
				}
			}
		}
		entities[i]->y += entities[i]->velocity_y * -cos(entities[i]->rotation) * FIXED_TIMESTEP;
		entities[i]->x += entities[i]->velocity_x * sin(entities[i]->rotation) * FIXED_TIMESTEP;
	}

	for (unsigned int i = 1; i < entities.size(); i++) {
		// Collision with player
		if (checkCollision(entities[i], entities[0])){
			Vector path = Vector((entities[i]->x - entities[0]->x), (entities[i]->y - entities[0]->y), 0.0f);

			//	Effect
			ParticleEmitter bumpParticles(50);
			bumpParticles.maxLifetime = 0.1f;
			bumpParticles.position = Vector(entities[0]->x + path.x * 0.15f, entities[0]->y + path.y * 0.15f, 0.0f);
			bumpParticles.velocity = Vector(path.x, path.y, 0.0f);
			bumpParticles.velocityDeviation = Vector(0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.0f);
			particleEmitters.push_back(bumpParticles);
			particleEmitters.back().effect();
		}

		for (unsigned int k = 0; k < MAX_BULLETS; k++) {
			if (bullets[k].visible && checkCollision(entities[i], &bullets[k])) {

				bullets[k].visible = false;

				if (entities[i]->scale_x < 0.99f) {
					//	Effect
					ParticleEmitter asteroidParticles(500);
					asteroidParticles.maxLifetime = 0.5f;
					asteroidParticles.position = Vector(entities[i]->x, entities[i]->y, 0.0f);
					asteroidParticles.velocity = Vector(sin(player->rotation)*bullets[k].velocity_x / 4, -cos(player->rotation)*bullets[k].velocity_y / 4, 0.0f);
					asteroidParticles.velocityDeviation = Vector(0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.0f);
					particleEmitters.push_back(asteroidParticles);
					particleEmitters.back().effect();

					//	Delete Entity (asteroid)
					delete entities[i];
					entities.erase(entities.begin() + i);
					break;
				}
				else {
					entities[i]->scale_x = entities[i]->scale_x * 0.7f;
					entities[i]->scale_y = entities[i]->scale_y * 0.7f;
					entities[i]->rotation = genRandomNum(-90.0f, 90.0f);

					//	Effect
					ParticleEmitter asteroidParticles(500);
					asteroidParticles.maxLifetime = 0.5f;
					asteroidParticles.position = Vector(entities[i]->x, entities[i]->y, 0.0f);
					asteroidParticles.velocity = Vector(sin(player->rotation)*bullets[k].velocity_x / 4, -cos(player->rotation)*bullets[k].velocity_y / 4, 0.0f);
					asteroidParticles.velocityDeviation = Vector(0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.5f + ((float)rand() / (float)RAND_MAX)*0.5f, 0.0f);
					particleEmitters.push_back(asteroidParticles);
					particleEmitters.back().effect();

					//	Splitting the Asteroid (Physical)
					SheetSprite asteroidSprite = SheetSprite(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
					Entity* asteroid = new Entity();
					asteroid->sprite = asteroidSprite;
					asteroid->scale_x = entities[i]->scale_x;
					asteroid->scale_y = entities[i]->scale_y;
					asteroid->x = entities[i]->x + genRandomNum(-0.2f, 0.2f);
					asteroid->y = entities[i]->y + genRandomNum(-0.2f, 0.2f);
					asteroid->rotation = genRandomNum(-90.0f, 90.0f);
					asteroid->velocity_x = entities[i]->velocity_x;
					asteroid->velocity_y = entities[i]->velocity_y;
					entities.push_back(asteroid);
				}
			}
		}
	}
}

void AsteroidsApp::Render() {
	//	Render Background
	background->Render();

	//	Render Entities
	for (unsigned int i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	//	Render Bullets
	for (unsigned int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}

	//	Render Particles
	for (unsigned int i = 0; i < particleEmitters.size(); i++){
		particleEmitters[i].Render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool AsteroidsApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	//	Completing FixedUpdates
	float FixedElapsed = elapsed + timeLeftOver;
	if (FixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		FixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (FixedElapsed >= FIXED_TIMESTEP) {
		FixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = FixedElapsed;

	Update(elapsed);
	Render();
	
	return done;
}

bool AsteroidsApp::checkCollision(Entity* entity1, Entity* entity2) {
	//	Entities should not collide with themselves
	if (entity1 == entity2){
		return false;
	}

	//	Build Matrices of Entities
	entity1->buildMatrix();
	entity2->buildMatrix();

	float minX, maxX, minY, maxY;

	//	Looking at Entity 1
	Vector e1TL = Vector(-entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector e1BL = Vector(-entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);
	Vector e1TR = Vector(entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector e1BR = Vector(entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);

	//	World Coordinates
	e1TL = entity1->matrix * e1TL;
	e1BL = entity1->matrix * e1BL;
	e1TR = entity1->matrix * e1TR;
	e1BR = entity1->matrix * e1BR;

	//	Entity2's Object Space
	Matrix e2Inverse = entity2->matrix.inverse();
	e1TL = e2Inverse * e1TL;
	e1BL = e2Inverse * e1BL;
	e1TR = e2Inverse * e1TR;
	e1BR = e2Inverse * e1BR;

	//	X-Axis Collisions
	minX = min(min(min(e1TL.x, e1BL.x), e1TR.x), e1BR.x);
	maxX = max(max(max(e1TL.x, e1BL.x), e1TR.x), e1BR.x);
	if (!(minX <= entity2->sprite.width * entity2->scale_x && maxX >= -entity2->sprite.width * entity2->scale_x)){
		return false;
	}

	//	Y-Axis Collisions
	minY = min(min(min(e1TL.y, e1BL.y), e1TR.y), e1BR.y);
	maxY = max(max(max(e1TL.y, e1BL.y), e1TR.y), e1BR.y);
	if (!(minY <= entity2->sprite.height * entity2->scale_y && maxY >= -entity2->sprite.height * entity2->scale_y)){
		return false;
	}

	//	Looking at Entity 2
	Vector e2TL = Vector(-entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector e2BL = Vector(-entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);
	Vector e2TR = Vector(entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector e2BR = Vector(entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);

	//	World Coordinates
	e2TL = entity2->matrix * e2TL;
	e2BL = entity2->matrix * e2BL;
	e2TR = entity2->matrix * e2TR;
	e2BR = entity2->matrix * e2BR;

	// Entity1's Object Space
	Matrix e1Inverse = entity1->matrix.inverse();
	e2TL = e1Inverse * e2TL;
	e2BL = e1Inverse * e2BL;
	e2TR = e1Inverse * e2TR;
	e2BR = e1Inverse * e2BR;

	//	X-Axis Collisions
	minX = min(min(min(e2TL.x, e2BL.x), e2TR.x), e2BR.x);
	maxX = max(max(max(e2TL.x, e2BL.x), e2TR.x), e2BR.x);
	if (!(minX <= entity1->sprite.width * entity1->scale_x && maxX >= -entity1->sprite.width * entity1->scale_x)){
		return false;
	}

	//	Y-Axis Collisions
	minY = min(min(min(e2TL.y, e2BL.y), e2TR.y), e2BR.y);
	maxY = max(max(max(e2TL.y, e2BL.y), e2TR.y), e2BR.y);
	if (!(minY <= entity1->sprite.height * entity1->scale_y && maxY >= -entity1->sprite.height * entity1->scale_y)){
		return false;
	}

	return true;
}

void AsteroidsApp::shootBullet() {
	//	Sound
	Mix_PlayChannel(-1, bullet, 0);

	//	"Spawning" Bullet
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = (player->x);
	bullets[bulletIndex].y = (player->y);
	bullets[bulletIndex].rotation = player->rotation;
	bullets[bulletIndex].velocity_x = 2.0f;
	bullets[bulletIndex].velocity_y = 2.0f;

	//	Checking if we need to reset bulletIndex
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}

	//	Restart the bullet cooldown
	bulletCD = 0;
}