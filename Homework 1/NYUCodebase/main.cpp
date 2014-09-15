/*	Name: Shunman Tse
Fall 2014 - CS3113 Intro to Game Programming
Assignment 1
*/

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

/* Assignment: Create a simple 2D scene using textured and untextures polygons
1. At least one element must be animated
2. At least 3 different textures must be used
3. At least one element must use vertex colors
*/

/* Plan: 2D space scene
1.	Have spinning candy stars!
*/


SDL_Window* displayWindow;

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

void DrawSprite(GLint texture, float x, float y, float rotation, float scale) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 1 - My Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	// Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	// Load Textures
	GLuint spinningLollipop = LoadTexture("lollipopRed.png");
	GLuint asteroid = LoadTexture("meteorBig.png");
	GLuint player = LoadTexture("player.png");
	GLuint enemy = LoadTexture("enemyShip.png");

	// Set up timing variables
	float lastFrameTicks = 0.0f;
	float starRotation = 0.0f;
	float asteroidRotation = 0.0f;
	float asteroid2Rotation = 45.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		// Keeping time in the while loop
		float ticks = (float)SDL_GetTicks()/ 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// Clear screen
		glClearColor(0.5f, 0.4f, 0.7f, 1.0f); // Purple Screen
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White Screen
		glClear(GL_COLOR_BUFFER_BIT);

		//-- Natural Elements 
		// Spinning Candy Star (top left)
		starRotation += (elapsed * 20.0f);
		DrawSprite(spinningLollipop, -1.1, 0.8, starRotation, 0.3);

		// Asteroids
		asteroidRotation += (elapsed * 30.0f);
		DrawSprite(asteroid, 0.0, -0.4, asteroidRotation, 0.4); 

		asteroid2Rotation += (elapsed * 35.0f);
		DrawSprite(asteroid, 0.6, -0.2, asteroid2Rotation, 0.3); 
		DrawSprite(asteroid, -0.6, -0.2, asteroid2Rotation, 0.3);
		DrawSprite(asteroid, 1.2, -0.2, 0, 0.5);
		DrawSprite(asteroid, -1.2, -0.2, 180, 0.5);
			
		//-- Player Forces
		// 2 Player Ships (bottom)
		DrawSprite(player, -0.5, -0.8, 0, 0.2);
		DrawSprite(player, 0.5, -0.8, 0, 0.2);

		//-- Enemy Forces
		// 5 Enemy Ships (top)
		DrawSprite(enemy, 0.0, 0.0, 0, 0.2);
		DrawSprite(enemy, 0.2, 0.2, 0, 0.2);
		DrawSprite(enemy, -0.2, 0.2, 0, 0.2);
		DrawSprite(enemy, 0.6, 0.4, 0, 0.2);
		DrawSprite(enemy, -0.6, 0.4, 0, 0.2);

		// Rainbow Capital Ship (triangle with vertex colors)
		// Resize the ship accordingly
		glLoadIdentity();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(0.0, 6.5, 0.0);

		// Array of vertex position data
		GLfloat capitalShip [] = { 0.0, 3.0, 1.0, 3.0, 3.0, 2.0, 2.0, 1.0, 1.0, -2.0, 0.0, -3.0, -1.0, -2.0, -2.0, 1.0, -3.0, 2.0, -1.0, 3.0 }; // 10 points 
		glVertexPointer(2, GL_FLOAT, 0, capitalShip);
		glEnableClientState(GL_VERTEX_ARRAY);

		// Array of Vertex Color Data
		GLfloat capitalShipColors[] = { 
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.5, 0.5, 0.5,
			0.0, 1.0, 0.0,
			1.0, 1.0, 1.0,
			0.0, 1.0, 0.0,
			0.5, 0.5, 0.5,
			0.0, 0.0, 1.0,
			1.0, 0.0, 0.0 };
		glColorPointer(3, GL_FLOAT, 0, capitalShipColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_POLYGON, 0, 10);
		glDisableClientState(GL_COLOR_ARRAY);
		
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
