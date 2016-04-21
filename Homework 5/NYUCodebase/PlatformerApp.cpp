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

float PlatformerApp::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

/*	This function sets up the basics for graphical displays on the screen.
	It also loads in all the texture
*/
PlatformerApp::PlatformerApp() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 5 - Side Scrolling Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//--- Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 800);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.66, 2.66, -2.0, 2.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//--- Load in textures
	spritesheet = LoadTexture("spritesheet.png");			// 819 x 21
	font = LoadTexture("font.png");							// 512 x 512

	//OutputDebugStringA("asd");

	Init();
}

/*	This function is run once at the start of the game.
	It populates the entities vector with players and walls of the game
*/
void PlatformerApp::Init() {

	// Add the player
	SheetSprite playerSprite = SheetSprite(spritesheet, 39, 1, 4);
	Entity* player = new Entity(playerSprite, -7.0f, 1.0f, 0.5f, 0.0f, 0.0f);
	entities.push_back(player);

	buildLevel();

}

void PlatformerApp::buildLevel() {
	ifstream infile("map.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
	}
}

bool PlatformerApp::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { 
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool PlatformerApp::readLayerData(ifstream& stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val-1;
					}
					else {
						levelData[y][x] = 0;
					}
				}

			}
		}
	}
	return true;
}

void PlatformerApp::RenderLevel(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spritesheet);

	vector<float> vertexData;
	vector<float> texCoordData;

	int numVertices = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {

				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x, -TILE_SIZE* y,
					TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});

				numVertices += 4;
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

PlatformerApp::~PlatformerApp() {
	SDL_Quit();
}

/*	This function renders the game objects onto the screen based on the state of the game
There are three states to consider: the main menu, the game level, and the game over screen
*/
void PlatformerApp::Render() {
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // Default Clear Screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Limit the view of the screen
	float Xlocation = -entities[0]->x;
	float Ylocation = -entities[0]->y;

	if (Ylocation > -0.5f) {
		Ylocation = -0.5f;
	}
	if (Xlocation > 4.83f) {
		Xlocation = 4.83f;
	}
	if (Xlocation < -4.83f) {
		Xlocation = -4.83f;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(Xlocation, Ylocation, 0.0f);

	// Render our entities
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	// Render the level
	RenderLevel();

	SDL_GL_SwapWindow(displayWindow);
}

//	Physics friendly update function

void PlatformerApp::FixedUpdate(){
	for (size_t i = 0; i < entities.size(); i++) {

		// Handle collision flags
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
			// Our entity hit something on the right side
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
					}
					else {
						entities[i]->y -= yPenetration + 0.001f;
						entities[i]->collidedTop = true;
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
					}
				}
			}
		}	
	}
}

void PlatformerApp::Update(float elapsed) {
	
	for (size_t i = 0; i < entities.size(); i++){
		// Kill the player for going out of bounds
		if (entities[i]->isPlayer){
			if (entities[i]->y < -2.35f || entities[i]->y > 2.35 || entities[i]->x > 2.1 || entities[i]->x < -2.1){
				entities[i]->playerDead = true;
			}
		}
	}

	// Handle playerDead -- send player back to initial position

}

void PlatformerApp::worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY) {
	*gridX = (int)((worldX) / TILE_SIZE);
	*gridY = (int)((-worldY) / TILE_SIZE);
}

/*
void PlatformerApp::tileToWorldCoordinatesX(float &x, int gridx){
	x = ((float)gridx)*TILE_SIZE;
}

void PlatformerApp::tileToWorldCoordinatesY(float &y, int gridy){
	y = -((float)gridy)*TILE_SIZE;
}
*/

bool PlatformerApp::UpdateAndRender() {

	// Debugging
	OutputDebugStringA("");

	float x_speed = 50.0f * FIXED_TIMESTEP;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	
	// Player 1
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A]) {
		entities[0]->velocity_x = -1.0f * x_speed * 10;
	}
	if (keys[SDL_SCANCODE_D]) {
		entities[0]->velocity_x = x_speed * 10;
	}
	if (keys[SDL_SCANCODE_S]) {
		entities[0]->velocity_x = 0.0f;
	}
	if (keys[SDL_SCANCODE_W]) {
		entities[0]->velocity_y = 1.0 * x_speed;
	}

	float fixedElapsed = elapsed + timeLeftOver;

	while (fixedElapsed >= FIXED_TIMESTEP){
		FixedUpdate();		
		fixedElapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;
	
	Update(elapsed);	
	Render();

	return done;
}


bool PlatformerApp::isSolid(unsigned char tile){
	// Returns whether something is solid or not based on their value in levelData
	switch (tile){
	case 11:
		return true;
		break;
	case 12:
		return true;
		break;
	case 13:
		return true;
		break;
	case 14:
		return true;
		break;
	case 31:
		return true;
		break;
	case 32:
		return true;
		break;
	case 33:
		return true;
		break;
	default:
		return false;
		break;
	}
}

float PlatformerApp::checkCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);

	if (isSolid(levelData[gridY][gridX])) {
		return 0.004f;
	}
	return 0.0f;
}

float PlatformerApp::checkCollisionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);

	if (isSolid(levelData[gridY][gridX])) {

		float yCoordinate = (gridY * TILE_SIZE) - (TILE_SIZE*16.0);
		return -y - yCoordinate;

	}
	return 0.0f;
}

void PlatformerApp::handleCollisionX(Entity *entity) {
	//check right

	float adjust = checkCollisionX(entity->x + TILE_SIZE*0.5, entity->y);
	if (adjust != 0.0f) {
		entity->x -= adjust;
		entity->velocity_x = 0.0f;
		entity->collidedRight = true;
	}

	//check left

	adjust = checkCollisionX(entity->x - TILE_SIZE*0.5, entity->y);
	if (adjust != 0.0f) {
		entity->x += adjust;
		entity->velocity_x = 0.0f;
		entity->collidedLeft = true;
	}

}

void PlatformerApp::handleCollisionY(Entity *entity) {
	//check bottom

	float adjust = checkCollisionY(entity->x, entity->y - TILE_SIZE*0.5);
	if (adjust != 0.0f) {
		entity->y += adjust;
		entity->velocity_y = 0.0f;
		entity->collidedBot = true;
	}
}


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