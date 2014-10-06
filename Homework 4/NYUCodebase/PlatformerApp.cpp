#include "PlatformerApp.h"

/*	Use SDL to load in textures. The function returns GLuint textureID.
*/
GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

/*
*/
float PlatformerApp::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

/*	This function sets up the basics for graphical displays on the screen.
It also loads in all the texture files and fills various vectors with proper elements.
*/
PlatformerApp::PlatformerApp() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 4 - Sinlge Screen Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//--- Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 800);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//--- Load in textures
	bg_texture = LoadTexture("background.png");				// 1024 * 1024
	//enemies_texture = LoadTexture("enemy.png");				// 51 * 51
	extras = LoadTexture("extras.png");						// 512 * 512
	font = LoadTexture("font.png");							// 512 * 512
	grass = LoadTexture("grassSheet.png");					// 512 * 512
	items = LoadTexture("items.png");						// 576 * 576
	player_texture = LoadTexture("player.png");				// 512 * 256

	Init();
}

/*	This function is run once at the start of the game.
It populates the entities vector with the player and enemy ships
*/
void PlatformerApp::Init() {
	lastFrameTicks = 0.0f;
	bulletIndex = 0;

	// Set up the background
	SheetSprite background_texture = SheetSprite(bg_texture, 0.0, 0.0, 1024.0f/1024.0f, 1024.0f/1024.0f);
	Entity* bg = new Entity(background_texture, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f);
	background.push_back(bg);

	// Set up the player 1 -- the sprite is the alien green square
	SheetSprite playerSprite = SheetSprite(player_texture, 288.0f / 512.0f, 144.0f / 256.0f, 70.0f / 512.0f, 70.0f / 256.0f);
	Entity* player = new Entity(playerSprite, -1.8f, -1.75f, 0.5f, 0.0f, 0.0f);
	player->isPlayer = true;
	entities.push_back(player);

	// Set up player 2
	SheetSprite enemySprite = SheetSprite(player_texture, 216.0f / 512.0f, 72.0f / 256.0f, 70.0f / 512.0f, 70.0f / 256.0f);
	Entity* player2 = new Entity(enemySprite, 1.8f, -1.75f, 0.5f, 0.0f, 0.0f);
	player2->isPlayer = true;
	entities.push_back(player2);

	// Set up the map
	// Load in Floor texture (slice03_03.png)
	SheetSprite floorSprite = SheetSprite(grass, 280.0f / 512.0f, 210.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f);

	// Walls - Left Side
	for (float i = -2.2f; i <= 2.2f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, -2.0f, i, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Walls - Right Side
	for (float i = -2.2f; i <= 2.2f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, 2.0f, i, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 0 - Bottom Floor (Left Side)
	for (float i = -2.2f; i <= -0.3f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, i, -1.95f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 0 - Bottom Floor (Right Side)
	for (float i = 0.3f; i <= 2.2f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, i, -1.95f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 1 - Left
	for (float i = -1.5f; i <= -0.8f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, i, -1.35f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 1 - Right
	for (float i = 0.8f; i <= 1.5f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, i, -1.35f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 2
	for (float i = -0.8f; i <= 0.8f; i += 0.1f) {
		Entity* floor = new Entity(floorSprite, i, -0.75f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 3 - Left
	for (float i = -2.2f; i < -0.4f; i += 0.1f){
		Entity* floor = new Entity(floorSprite, i, -0.15f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}

	// Level 3 - Right
	for (float i = 0.4f; i < 2.2f; i += 0.1f){
		Entity* floor = new Entity(floorSprite, i, -0.15f, 0.5f, 0.0f, 0.0f);
		floor->isStatic = true;
		entities.push_back(floor);
	}
}
/*
void PlatformerApp::spawnEnemy() {

	float enemyY = ((rand() % 401) / 100.0f) - 2.0f;
	float enemyX;

	SheetSprite enemySprite = SheetSprite(enemies_texture, 65.0f / 256.0f, 0.0f, 51.0f / 256.0f, 51.0f / 128.0f); // green block texture

	// Change the value of enemyX based on enemyDirection
	if (enemyDirection) {
		enemyX = 1.0f;
		enemyDirection = false;
	}
	else {
		enemyX = -1.0f;
		enemyDirection = true;
	}

	numEnemy += 1;
	Entity* enemy = new Entity(enemySprite, 0.0f, 2.1f, 0.25f, enemyX, 0.0f);
	enemy->isEnemy = true;
	entities.push_back(enemy);
}

void PlatformerApp::spawnEnemy() {
	SheetSprite enemySprite = SheetSprite(player_texture, 216.0f/512.0f, 72.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f); // green block texture
	Entity* enemy = new Entity(enemySprite, 0.0f, 2.1f, 0.4f, 0.0f, 0.0f);
	enemy->isEnemy = true;
	entities.push_back(enemy);
}
*/

PlatformerApp::~PlatformerApp() {
	SDL_Quit();
}

/*	This function renders the game objects onto the screen based on the state of the game
There are three states to consider: the main menu, the game level, and the game over screen
*/
void PlatformerApp::Render() {
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // Blue Screen
	glClear(GL_COLOR_BUFFER_BIT);

	background[0]->Render();

	// Put up the Scores!
	glLoadIdentity();
	glTranslatef(-1.8f, 0.87f, 0.0f);
	Text(font, "P1 Score: " + to_string(entities[0]->score), 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(0.0f, 0.87f, 0.0f);
	Text(font, "P2 Score: " + to_string(entities[1]->score), 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

	// Render our entities
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

void PlatformerApp::FixedUpdate(){
	for (size_t i = 0; i < entities.size(); i++) {

		// Handle collisions
		if (entities[i]->collidedBot) {
			// Our entity hit something on its bottom
			entities[i]->inJump = false;	// the entity is no longer jumping
			entities[i]->velocity_y = 0.0f; // stop the entity
			entities[i]->acceleration_y = 0.0f;
		}
		if (entities[i]->collidedTop) {
			// Our entity bumped something with its head (top)
			entities[i]->velocity_y = 0.0f; // Let gravity take over
			entities[i]->acceleration_y = 0.0f;
		}
		if (entities[i]->collidedLeft) {
			// Our entity hit something on its left side
			entities[i]->velocity_x = 0.0f;
			entities[i]->acceleration_x = 0.0f;
		}
		if (entities[i]->collidedRight) {
			entities[i]->velocity_x = 0.0f; // prevent the entity from going through wall
			entities[i]->acceleration_x = 0.0f;
		}

		// Reset collision flags
		entities[i]->collidedBot = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		// Apply gravity to all NON-static entities
		if (!(entities[i]->isStatic)) {
			entities[i]->velocity_x -= gravity_x * FIXED_TIMESTEP;	// for this program, the x gravity is 0.0f
			entities[i]->velocity_y -= gravity_y * FIXED_TIMESTEP;
		}

		// Apply friction to all entities
		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		// Apply acceleration to all entities's velocities
		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		// Update y and check for collisions
		entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;

		if (!(entities[i]->isStatic)){
			for (size_t j = 0; j < entities.size(); j++){
				if (entities[i]->collidesWith(entities[j]) && (entities[i] != entities[j])) {
					// There is a collision and it's not the entity interacting with itself
					float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - entities[i]->sprite.height / 2.0f - entities[j]->sprite.height / 2.0f);
					
					if (entities[i]->y > entities[j]->y) {
						entities[i]->y += yPenetration + 0.001f;
						entities[i]->collidedBot = true;

						if (entities[i]->isPlayer){
							if (entities[j]->isLoot){
								if (!entities[j]->looted) {
									entities[i]->score += points;
								}
								entities[j]->looted = true;
							}
							else if (entities[j]->isPlayer) {
								entities[j]->playerDead = true;
								entities[i]->score += 100;
								entities[i]->Jump();
							}
						}

						if (entities[i]->isLoot){
							if (entities[j]->isPlayer){
								if (!entities[i]->looted) {
									entities[j]->score += points;
								}
								entities[i]->looted = true;
							}
						}
					}
					else {
						entities[i]->y -= yPenetration + 0.001f;
						entities[i]->collidedTop = true;

						if (entities[j]->isPlayer){
							if (entities[i]->isPlayer) {
								entities[i]->playerDead = true;
								entities[j]->score += 100;
								entities[j]->Jump();
							}
							else if (entities[j]->isLoot){
								if (!entities[j]->looted) {
									entities[i]->score += points;
								}
								entities[j]->looted = true;
								entities[i]->score += points;
							}
						}

					}
				}
			}
		}

		// Update x and check for collisions
		entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;

		if (!(entities[i]->isStatic)){
			for (size_t j = 0; j < entities.size(); j++){
				if (entities[i]->collidesWith(entities[j]) && (entities[i] != entities[j])) {
					float xPenetration = fabs(fabs(entities[j]->x - entities[i]->x) - entities[i]->sprite.width / 2.0f - entities[j]->sprite.width / 2.0f);

					if (entities[i]->x > entities[j]->x) {
						entities[i]->x += xPenetration + 0.001f;
						entities[i]->collidedLeft = true;
	
						if (entities[j]->isLoot && entities[i]->isPlayer){
							if (!entities[j]->looted) {
								entities[i]->score += points;
							}
							entities[j]->looted = true;
						}
						else if ((entities[i]->isLoot && entities[j]->isPlayer)) {
							if (!entities[i]->looted) {
								entities[j]->score += points;
							}
							entities[i]->looted = true;
						}
					}
					else if (entities[i]->x < entities[j]->x) {
						entities[i]->x -= xPenetration + 0.001f;
						entities[i]->collidedRight = true;

						if (entities[j]->isLoot && entities[i]->isPlayer){
							if (!entities[j]->looted) {
								entities[i]->score += points;
							}
							entities[j]->looted = true;
						}
						else if ((entities[i]->isLoot && entities[j]->isPlayer)) {
							if (!entities[i]->looted) {
								entities[j]->score += points;
							}
							entities[i]->looted = true;
						}
					}
				}
			}
		}	


	}
} 

/*	
*/
void PlatformerApp::Update(float elapsed) {
	
	for (size_t i = 0; i < entities.size(); i++){
		// Kill the player for going out of bounds
		if (entities[i]->isPlayer){
			if (entities[i]->y < -2.35f || entities[i]->y > 2.35 || entities[i]->x > 2.1 || entities[i]->x < -2.1){
				entities[i]->playerDead = true;
			}
		}

		// Consider the coin looted if it's out of bounds
		if (entities[i]->isLoot){
			if (entities[i]->y < -2.35f || entities[i]->y > 2.35 || entities[i]->x > 2.1 || entities[i]->x < -2.1){
				entities[i]->looted = true;
			}
		}

		// Making da bling go away if looted
		if (entities[i]->isLoot){
			if (entities[i]->looted){
				delete entities[i];
				entities.erase(entities.begin() + i);
				numLoot -= 1;
			}
		}

		// Handle the PKing
		else if (entities[i]->isPlayer){
			if (entities[i]->playerDead){
				if (i == 0) {
					entities[i]->x = -1.8f;
					entities[i]->y = -1.75f;
					entities[i]->playerDead = false;
				}
				else {
					entities[i]->x = 1.8f;
					entities[i]->y = -1.75f;
					entities[i]->playerDead = false;
				}
			}
		}


	}

	if (numLoot <= MAX_LOOT && spawnTimer > 0.2f) {
	SheetSprite lootSprite = SheetSprite(items, 288.0f / 576.0f, 360.0f / 576.0f, 70.0f / 576.0f, 70.0f / 576.0f); // gold coin texture
	Entity* loot = new Entity(lootSprite, 0.0f, 2.1f, 0.8f, 0.0f, 0.0f);

	loot->x = float (float (rand() % 4000) / 1000.0f) - 2.0f;
	loot->y = float(float(rand() % 3000) / 1000.0f) - 1.5f;
	loot->isLoot = true;
	loot->looted = false;
	entities.push_back(loot);

	numLoot += 1;
	spawnTimer = 0.0f;
	}

	spawnTimer += elapsed;

}

/*	Game loop
	Handle player inputs here
*/
bool PlatformerApp::UpdateAndRender() {

	float x_speed = 0.9f / 6.0f;

	// First we update the timing of the game
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		// Check whether the close button for the game has been pressed -- if so, end the game
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	
	// Player 1
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A]) {
		// Move Left
		entities[0]->velocity_x = -1.0f * x_speed;
		//entities[0]->acceleration_x = -0.5 / 10.0f;
	}
	if (keys[SDL_SCANCODE_D]) {
		// Move Right
		entities[0]->velocity_x = x_speed;
		//entities[0]->acceleration_x = 0.5 / 10.0f;
	}
	if (keys[SDL_SCANCODE_S]) {
		// All stop
		entities[0]->velocity_x = 0.0f;
		//entities[0]->acceleration_x = 0.0f;
	}
	if (keys[SDL_SCANCODE_W]) {
		// Jump
		if (entities[0]->collidedBot) {
			// We can jump because we're not in the middle of a jump
			entities[0]->Jump();
		}
	}
	
	// Player 2
	if (keys[SDL_SCANCODE_LEFT]) {
		// Move Left
		entities[1]->velocity_x = -1.0f * x_speed;
		//entities[0]->acceleration_x = -0.5 / 10.0f;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		// Move Right
		entities[1]->velocity_x = x_speed;
		//entities[0]->acceleration_x = 0.5 / 10.0f;
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		// All stop
		entities[1]->velocity_x = 0.0f;
		//entities[0]->acceleration_x = 0.0f;
	}
	if (keys[SDL_SCANCODE_UP]) {
		// Jump
		if (entities[1]->collidedBot) {
			// We can jump because we're not in the middle of a jump
			entities[1]->Jump();
		}
	}

	// Call Update and Render
	FixedUpdate();		// Detect collisions and set flags
	Update(elapsed);	// Handle collisions
	Render();

	return done;
}

/*	This function will print strings onto the screen
*/
void PlatformerApp::Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a) {
	// Have to go character by character for the offsets...
	// The 16 is the width of the sprite sheet in terms of the number of sprites
	// We must draw multiple quads -- more complex than a single square -- dynamic mesh

	glBindTexture(GL_TEXTURE_2D, Texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	// We must build vertex by vertex for each quad ...
	// We must account for the offset between letters by using size*i
	// We're transforming manually. We're drawing all the quads in a single draw column.
	// If we used the modelview matrix, the entire thing is affected. We don't want that...
	float texture_size = 1.0f / 16.0f;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), (0.5f * size),
			((size + spacing) * i) + (-0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (0.5f * size) });
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		uvData.insert(uvData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}