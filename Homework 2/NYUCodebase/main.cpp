/*	Name: Shunman Tse
Fall 2014 - CS3113 Intro to Game Programming
Assignment 2: Create PONG
*/

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "main.h"

SDL_Window* displayWindow;

//--- Global Variables / Definitions
#define PI 3.14159265

Entity ball;	// this is the ball ...
Entity paddle1; // green player -- left side
Entity paddle2; // red player -- right side
Entity topWall; // wall at the top of the screen
Entity botWall; // wall at the bottom of the screen

/* Use SDL to load in textures. The function returns GLuint textureID.
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

/*	Function that will test for a collision between the parameter paddle and a wall (top & bottom)
	Return 1 for collision with topWall, 2 for collision with botWall, 0 by default
*/
int paddleCollisionCheck(Entity paddle) {

	//--- Check for paddle collisions
	if ((paddle.y + ((paddle.height * paddle.scale)/2)) > topWall.y) {
		// paddle is colliding with the topWall
		return 1;
	}
	else if ((paddle.y - ((paddle.height * paddle.scale) / 2)) < botWall.y) {
		// paddle is colliding with the botWall
		return 2;
	}

	return 0;
}

int ballCollisionCheck() {
	// All in one function that checks whether the ball is colliding with one of the paddles or one of the walls

	if ((ball.x - (ball.width * ball.scale /2) <= paddle1.x + (paddle1.width * paddle1.scale / 2)) && (ball.x - (ball.width * ball.scale/2) >= (paddle1.x - (paddle1.width * paddle1.scale / 2)))) {
		if ((ball.y <= (paddle1.y + (paddle1.height * paddle1.scale / 2))) && (ball.y >= (paddle1.y - (paddle1.height * paddle1.scale / 2)))) {
			// This is a collision between ball and paddle1
			return 1;
		}
	}

	else if ((ball.x + (ball.width * ball.scale / 2) >= paddle2.x - (paddle2.width * paddle2.scale / 2)) && (ball.x + (ball.width * ball.scale / 2) <= (paddle2.x + (paddle2.width * paddle2.scale / 2)))) {
		if ((ball.y <= (paddle2.y + (paddle2.height * paddle2.scale / 2))) && (ball.y >= (paddle2.y - (paddle2.height * paddle2.scale / 2)))) {
			// This is a collision between ball and paddle2
			return 2;
		}
	}
	else if (ball.y + (ball.height * ball.scale / 2) >= topWall.y) {
		return 3;
	}

	else if (ball.y - (ball.height * ball.scale / 2) <= botWall.y) {
		return 3;
	}
}

void Setup(){
	// Setup SDL & OpenGL
	// Set projection matrix
	// Load Textures & Assign to Proper Entity

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 2 - PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	// Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//--- Load Textures (HW2 Code)
	GLuint ballImg = LoadTexture("enemyUFO.png");
	GLuint paddle1Img = LoadTexture("laserBlue02.png");
	GLuint paddle2Img = LoadTexture("laserRed02.png");
	GLuint wallImg = LoadTexture("laserGreen03.png");

	// Setting Entity textureIDs
	ball.textureID = ballImg;
	paddle1.textureID = paddle1Img;
	paddle2.textureID = paddle2Img;
	topWall.textureID = wallImg;
	botWall.textureID = wallImg;
}

bool ProcessEvents(){
	// our SDL event loop
	// check input events

	SDL_Event event;

	int collision = ballCollisionCheck();

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}

		if (collision == 1) { // collision with paddle1
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					// detect W press
					if (ball.velocity_y == 0.0f) {
						ball.velocity_y = 0.9f;
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					// detect S press
					if (ball.velocity_y == 0.0f) {
						ball.velocity_y = -0.9f;
					}
				}
			}

			else {
				ball.velocity_x *= -1.0f;
			}
		}

		else if (collision == 2) { // collision with paddle2
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					// detect UP press
					if (ball.velocity_y == 0.0f) {
						ball.velocity_y = 0.9f;
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					// detect DOWN press
					if (ball.velocity_y == 0.0f) {
						ball.velocity_y = -0.9f;
					}
				}
			}

			else {
				ball.velocity_x *= -1.0f;
			}
		}

		return false;
	}
}

void initEntityValues(){

	// Ball Settings
	ball.scale = 0.08f;
	ball.velocity_x = -1.0f;
	ball.velocity_y = 0.0f;
	ball.speed = 0.9f;

	// Paddle Settings
	float paddle_speed = 0.9f;
	float paddle_height = 5.5f;
	float paddle_scale = 0.1f;
	float paddle_width = 0.5f;

	paddle1.height = paddle_height;
	paddle1.width = paddle_width;
	paddle1.scale = paddle_scale;
	paddle1.speed = paddle_speed;
	paddle1.x = -1.0f;

	paddle2.height = paddle_height;
	paddle2.width = paddle_width;
	paddle2.scale = paddle_scale;
	paddle2.speed = paddle_speed;
	paddle2.x = 1.0f;

	// Wall Settings
	topWall.width = 30.0f;
	topWall.scale = 0.1f;
	topWall.height = 0.5f;
	topWall.y = 0.9f;

	botWall.width = 30.0f;
	botWall.scale = 0.1f;
	botWall.height = 0.5f;
	botWall.y = -0.9f;
}

/* This function will generate the sprites of the ball, paddles, and walls. 
*/
void Render(){
	
	// Clear screen
	glClearColor(0.5f, 0.4f, 0.7f, 1.0f); // Purple Screen
	glClear(GL_COLOR_BUFFER_BIT);

	ball.Draw();
	paddle1.Draw();
	paddle2.Draw();
	topWall.Draw();
	botWall.Draw();

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup(){
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	Setup();
	initEntityValues();

	bool done = false;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	//--- Time Keeping
	float lastFrameTicks = 0.0f;

	while (!done) {

		done = ProcessEvents();

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// Process Ball Movement
		ball.x += ball.velocity_x * elapsed;
		ball.y += ball.velocity_y * elapsed;

		// Read for Input On Left Player (W & S)
		if (keys[SDL_SCANCODE_W]) {
			if (paddleCollisionCheck(paddle1) != 1) {
				paddle1.y += (elapsed * paddle1.speed);
			}
		}
		else if (keys[SDL_SCANCODE_S]) {
			if (paddleCollisionCheck(paddle1) != 2) {
				paddle1.y -= (elapsed * paddle1.speed);
			}
		}

		// Read for Input On Right Player (UP & DOWN)
		if (keys[SDL_SCANCODE_UP]) {
			if (paddleCollisionCheck(paddle2) != 1) {
				paddle2.y += (elapsed * paddle2.speed);
			}
		}
		else if (keys[SDL_SCANCODE_DOWN]){
			if (paddleCollisionCheck(paddle2) != 2) {
				paddle2.y -= (elapsed * paddle2.speed);
			}
		}

		Render();
	}

	Cleanup();
	return 0;
}




/*
if (ballCollision == 1) { // Collided with paddle1
ball.direction_x *= -1.0f;
ball.direction_y -= (paddle1.y);
}
else if (ballCollision == 2) { // Collided with paddle2
ball.direction_x *= -1.0f;
ball.direction_y -= (paddle2.y);
}
else if (ballCollision == 3) { // Collided with one of the walls (top or bottom)
ball.direction_y *= -1.0f;
}

ball.x += (elapsed * ball.direction_x * ball.speed);
ball.y += (elapsed * ball.direction_y * ball.speed);
*/

/*

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


*/

/*
if (ballCollisionCheck() == 1) { // collision with paddle1
if (event.type == SDL_KEYDOWN) {
if (event.key.keysym.scancode == SDL_SCANCODE_W) {
// detect W press
if (ball.velocity_y == 0.0f) {
ball.velocity_y = 0.9f;
}
}
else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
// detect S press
if (ball.velocity_y == 0.0f) {
ball.velocity_y = -0.9f;
}
}
}
}

else if (ballCollisionCheck() == 2) { // collision with paddle2
if (event.type == SDL_KEYDOWN) {
if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
// detect UP press
if (ball.velocity_y == 0.0f) {
ball.velocity_y = 0.9f;
}
}
}

else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
// detect DOWN press
if (ball.velocity_y == 0.0f) {
ball.velocity_y = -0.9f;
}
}
}
*/