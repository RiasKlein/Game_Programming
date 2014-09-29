#include "SpaceInvadersApp.h"

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

SpaceInvadersApp::SpaceInvadersApp() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 3 - Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//--- Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//--- Load in textures
	bg_textures = LoadTexture("backgrounds.png");			// 2048 x 4096
	font = LoadTexture("font.png");							// 512 x 512
	fg_textures = LoadTexture("foreground_assets.png");		// 1024 x 1024

	//--- Setup Background Entities
	// Have different backgrounds for each of the three game states of the game
	SheetSprite main_menuSprite = SheetSprite(bg_textures, 0.0f, 0.0f, 1600.0f / 2048.0f, 1200.0f / 4096.0f);
	Entity* main_menu = new Entity(main_menuSprite, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	backgrounds.push_back(main_menu);

	SheetSprite game_levelSprite = SheetSprite(bg_textures, 0.0f, 1202.0f / 4096.0f, 1600.0f / 2048.0f, 1200.0f / 4096.0f);
	Entity* game_level = new Entity(game_levelSprite, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	backgrounds.push_back(game_level);

	SheetSprite game_overSprite = SheetSprite(bg_textures, 0.0f, 2404.0f / 4096.0f, 1600.0f / 2048.0f, 1200.0f / 4096.0f);
	Entity* game_over = new Entity(game_overSprite, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	backgrounds.push_back(game_over);

	// Add Logo to backgrounds
	SheetSprite logoSprite = SheetSprite(bg_textures, 0.0f, 3606.0f / 4096.0f, 895.0f / 2048.0f, 387.0f / 4096.0f);
	Entity* logo = new Entity(logoSprite, 0.0f, 0.5f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f);
	backgrounds.push_back(logo);

	//--- Setup gameover scene
	SheetSprite mainBaseSprite = SheetSprite(fg_textures, 0.0f, 0.0f, 502.0f / 1024.0f, 481.0f / 1024.0f);
	Entity* mainBase = new Entity(mainBaseSprite, -0.2f, 0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.push_back(mainBase);

	// Push 2 explosion objects into scene
	SheetSprite boomSprite = SheetSprite(fg_textures, 632.0f / 1024.0f, 600.0f / 1024.0f, 121.0f / 1024.0f, 118.0f / 1024.0f);
	for (int i = 0; i <= 5; i++){
		Entity* boom = new Entity(boomSprite, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		scene.push_back(boom);
	}

	Init();
	done = false;
	score = 0;
	state = STATE_MAIN_MENU;
}

void SpaceInvadersApp::ReInit() {
	laserIndex = 0;

	// Make sure that previous game's elements are cleared
	for (unsigned int i = 0; i < MAX_LASERS; i++){
		lasers[i].visible = false;
	}

	if (entities.size() <= 18){
		// Set up the enemies
		SheetSprite enemySprite = SheetSprite(fg_textures, 632.0f / 1024.0f, 720.0f / 1024.0f, 106.0f / 1024.0f, 181.0f / 1024.0f);

		// We want to have enemies from -1.0 -> 1.0 (with the screen being -1.33 -> 1.33)
		for (float i = -1.0f; i <= 1.0f; i += 0.4f) {
			// We want to have multiple rows of these enemies
			for (float j = 0.7f; j >= 0.0f; j -= 0.3f) {
				Entity* enemy = new Entity(enemySprite, i, j, 0.5f, 180.0f, 0.1f, 0.2f, 0.025f);
				entities.push_back(enemy);
			}
		}
	}
}

void SpaceInvadersApp::Init() {
	lastFrameTicks = 0.0f;
	laserIndex = 0;

	//--- Setup Foreground Entities
	// Set up the player 
	SheetSprite playerSprite = SheetSprite(fg_textures, 504.0f / 1024.0f, 0.0f / 1024.0f, 452.0f / 1024.0f, 320.0f / 1024.0f);
	Entity* player = new Entity(playerSprite, 0.0f, -0.78f, 0.4f, 90.0f, 0.1f, 0.0f, 0.0f);
	entities.push_back(player);

	// Set up the enemies
	SheetSprite enemySprite = SheetSprite(fg_textures, 632.0f / 1024.0f, 720.0f / 1024.0f, 106.0f / 1024.0f, 181.0f / 1024.0f);

	// We want to have enemies from -1.0 -> 1.0 (with the screen being -1.33 -> 1.33)
	for (float i = -1.0f; i <= 1.0f; i += 0.4f) {
		// We want to have multiple rows of these enemies
		for (float j = 0.7f; j >= 0.0f; j -= 0.3f) {
			Entity* enemy = new Entity(enemySprite, i, j, 0.5f, 180.0f, 0.1f, 0.2f, 0.025f);
			entities.push_back(enemy);
		}
	}	
}

SpaceInvadersApp::~SpaceInvadersApp() {
	SDL_Quit();
}

void SpaceInvadersApp::Render() {
	
	//glClearColor(0.5f, 0.4f, 0.7f, 1.0f); // Purple Screen
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
	case STATE_MAIN_MENU:
		// Reset the position of the player ship (if one of the endgame screens was recently displayed)
		entities[0]->x = 0.0f;
		entities[0]->y = -0.78f;
		entities[0]->rotation = 90.0f;

		// Display Background
		backgrounds[STATE_MAIN_MENU]->Render();
		backgrounds[3]->Render();

		// Display Foreground (Text)
		glLoadIdentity();
		glTranslatef(-1.0f, -0.15f, 0.0f);
		Text(font, "High Score: " + to_string(high_score), 0.2f, -0.095f, 1.0f, 1.0f, 0.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-1.0f, -0.5f, 0.0f);
		Text(font, "Press ENTER to Start", 0.2f, -0.095f, 1.0f, 1.0f, 0.0f, 1.0f);

		break;

	case STATE_GAME_LEVEL:
		//--- Display Background (With the score)

		// Display background
		backgrounds[STATE_GAME_LEVEL]->Render();

		// Display score
		glLoadIdentity();
		glTranslatef(-1.0f, 0.9f, 0.0f);
		Text(font, "Score: " + to_string(score), 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);

		//--- Display Foreground

		// Display enemy ships and player ship
		for (unsigned int i = 0; i < entities.size(); i++) {
			entities[i]->Render();
		}

		// Display lasers (attacks)
		for (unsigned int i = 0; i < MAX_LASERS; i++) {
			lasers[i].Render();
		}

		break;

	case STATE_GAME_OVER:
		// Display Background
		backgrounds[STATE_GAME_OVER]->Render();

		// Display Foreground
		if (victory) { 
			// Player WON! -- Display the scene of the player ship returning back to base
			scene[0]->Render();
			entities[0]->x = 0.5f;
			entities[0]->y = 0.7f;
			entities[0]->rotation = 225.0f;
			entities[0]->Render();

			// Display the score
			glLoadIdentity();
			glTranslatef(-1.2f, 0.87f, 0.0f);
			Text(font, "Score: " + to_string(score), 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

			// Set high score if applicable
			if (score > high_score){
				high_score = score;
			}

			// Display the victory message
			glLoadIdentity();
			glTranslatef(-1.15f, -0.5f, 0.0f);
			Text(font, "CONGRATULATIONS, YOU WIN!", 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

			// Display the instructions to head back to 
			glLoadIdentity();
			glTranslatef(-1.0f, -0.75f, 0.0f);
			Text(font, "Press ENTER to Exit", 0.2f, -0.095f, 1.0f, 1.0f, 0.0f, 1.0f);
		}
		else { 
			// Player LOST! -- Display the scene of the player base on fire
			scene[0]->Render();		// Player base
			scene[1]->Render();		// Right side of base explosion
			scene[2]->x = -0.4f;	
			scene[2]->Render();		// Left side of base explosion
			scene[3]->x = -0.2f;
			scene[3]->y = -0.15f;
			scene[3]->Render();		// Bottom of base explosion
			scene[4]->x = 1.2f;
			scene[4]->y = -0.8f;
			scene[4]->Render();		// Bottom right of planet
			scene[5]->x = 1.1f;
			scene[5]->y = -0.3f;
			scene[5]->Render();		// Top right of planet

			// Display the score
			glLoadIdentity();
			glTranslatef(-1.2f, 0.87f, 0.0f);
			Text(font, "Score: " + to_string(score), 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

			// Set high score if applicable
			if (score > high_score){
				high_score = score;
			}

			// Display the victory message
			glLoadIdentity();
			glTranslatef(-1.15f, -0.5f, 0.0f);
			Text(font, "YOU'VE FAILED, GAME OVER!", 0.2f, -0.1f, 1.0f, 1.0f, 0.0f, 1.0f);

			// Display the instructions to head back to 
			glLoadIdentity();
			glTranslatef(-1.0f, -0.75f, 0.0f);
			Text(font, "Press ENTER to Exit", 0.2f, -0.095f, 1.0f, 1.0f, 0.0f, 1.0f);
		}
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool SpaceInvadersApp::CollisionCheck(Entity& entity, Laser& laser) {
	// Check for collision between the entity and laser

	float entity_top = entity.y + (entity.sprite.height / 2.0f);
	float entity_bot = entity.y - (entity.sprite.height / 2.0f);
	float entity_right = entity.x + (entity.sprite.width / 2.0f);
	float entity_left = entity.x - (entity.sprite.width / 2.0f);

	float laser_top = laser.y + (laser.sprite.height / 2.0f);
	float laser_bot = laser.y - (laser.sprite.height / 2.0f);
	float laser_right = laser.x + (laser.sprite.width / 2.0f);
	float laser_left = laser.x - (laser.sprite.width / 2.0f);

	if (entity_bot > laser_top) {
		return false;
	}
	else if (entity_top < laser_bot) {
		return false;
	}
	else if (entity_left > laser_right){
		return false;
	}
	else if (entity_right < laser_left) {
		return false;
	}

	return true;
}

void SpaceInvadersApp::Update_Game(float elapsed){

	// Alien Invaders' Turn
	for (unsigned int i = 1; i < entities.size(); i++){
		
		// Check if the aliens have won by reaching the bottom of the map
		if (entities[i]->get_y() < -0.5f){
			state = STATE_GAME_OVER;
		}

		// Change the direction of the aliens if they hit either the left or right map edges & move forward
		if ((entities[i]->get_x() > 1.25) || (entities[i]->get_x() < -1.25)) {
			for (unsigned int j = 1; j < entities.size(); j++){
				entities[j]->velocity_x *= -1.0f;
				entities[j]->y -= entities[j]->velocity_y;
			}
			break;
		}

		// Delete alien ships that are hit by player laser
		for (unsigned int j = 0; j < MAX_LASERS; j++){
			if (lasers[j].visible && CollisionCheck(*entities[i], lasers[j])) {
				// We have a collision!
				score += pts_per_enemy;
				lasers[j].visible = false; // So the same laser won't hit another ship
				delete entities[i];
				entities.erase(entities.begin() + i);
				break;
			}
		}
	}

	// Move the alien invaders
	for (unsigned int i = 1; i < entities.size(); i++){
		entities[i]->Update(elapsed);
	}

	// Move the lasers
	for (unsigned int i = 0; i < MAX_LASERS; i++){
		lasers[i].Update(elapsed);
	}

	// Check if the player has won
	if (entities.size() <= 1) {
		victory = true;
		state = STATE_GAME_OVER;
	}

	laser_cd -= elapsed;
}

void SpaceInvadersApp::shootLaser(){
	SheetSprite laserSprite = SheetSprite(fg_textures, 665.0f / 1024.0f, 322.0f / 1024.0f, 71.0f / 1024.0f, 120.0f / 1024.0f);
	lasers[laserIndex].sprite = laserSprite;
	
	lasers[laserIndex].x = entities[0]->get_x();
	lasers[laserIndex].y = -0.8f;
	lasers[laserIndex].scale = 1.0f;
	lasers[laserIndex].rotation = 0.0f;
	lasers[laserIndex].speed = 2.0f;
	lasers[laserIndex].visible = true;

	laserIndex++;

	if (laserIndex > (MAX_LASERS - 1)) {
		laserIndex = 0;
	}

	laser_cd = 0.2f;
}

void SpaceInvadersApp::Update(float elapsed) {
	SDL_Event event;
	
	switch (state){
	case STATE_MAIN_MENU:
		score = 0;
		victory = false;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					state = STATE_GAME_LEVEL;
				}
			}
		}

		break;
	case STATE_GAME_LEVEL:
		// Process Player Movement -- ONLY player speed matters
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_A) {
					if ((entities[0]->get_x()) > (-1.2f)) {
						entities[0]->x -= (entities[0]->speed);
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
					if ((entities[0]->get_x()) < (1.2f)) {
						entities[0]->x += (entities[0]->speed);
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_W){
					if (laser_cd <= 0.0f) {
						shootLaser();
					}
				}
			}
		}

		Update_Game(elapsed);

		break;
	case STATE_GAME_OVER:
		ReInit(); 

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					state = STATE_MAIN_MENU;
				}
			}
		}

		break;
	}
}

/* This is the game loop. It will be run continuously in a while loop in the main function of main.cpp
*/
bool SpaceInvadersApp::UpdateAndRender() {

	// First we update the timing of the game
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	// Call Update and Render
	Update(elapsed);
	Render();

	return done;
}

void SpaceInvadersApp::Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a) {
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
		float texture_x = (float) (((int)text[i]) % 16) / 16.0f;
		float texture_y = (float) (((int)text[i]) / 16) / 16.0f;

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