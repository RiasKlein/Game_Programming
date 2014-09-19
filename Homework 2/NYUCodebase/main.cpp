/*	Name: Shunman Tse
	Fall 2014 - CS3113 Intro to Game Programming
	Assignment 2: Create PONG
*/

/*	Assignment Details:
	- Make PONG!
	- Doesn't need to keep score
	- It must detect player wins
	- Can use images or basic shapes
	- Can use keyboard, mouse, or joystick

	Regarding this assignment:
	- This version of PONG requires 2 players (local). 
	- The left paddle is controlled by W for up and S for down.
	- The right paddle is controlled by the UP key for up and the DOWN key for down.
	- A winner logo pops on the screen when someone wins; The game must be restarted to play again
*/

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "main.h"
#include <cmath>

SDL_Window* displayWindow;

//--- Global Variables / Definitions
Entity ball;		// this is the ball ...
Entity paddle1;		// blue player -- left side
Entity paddle2;		// red player  -- right side
Entity topWall;		// wall at the top of the screen
Entity botWall;		// wall at the bottom of the screen
Entity winLogo;		// logo to display when someone wins

const Uint8 *keys = SDL_GetKeyboardState(NULL);

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

/*	Test for a collision between the parameter paddle and a wall (top & bottom)
	Returns an int: 1 for collision with topWall, 2 for collision with botWall, 0 by default
	*This function will ensure that the players' paddles do not cross the boundary of the playing field*
*/
int paddleCollisionCheck(Entity paddle) {

	// Check for a collision with the top wall
	if ((paddle.y + ((paddle.height * paddle.scale)/2)) > topWall.y) {
		return 1;
	}
	// Check for a collision with the bottom wall
	else if ((paddle.y - ((paddle.height * paddle.scale) / 2)) < botWall.y) {
		return 2;
	}
	return 0;
}

/*	Test for a collision between the ball and paddles or walls
	Returns an int: 1 for a collision with paddle1, 2 for a collision with paddle2, 3 for a collision with a wall, 0 by default
*/
int ballCollisionCheck() {
	// Account for the fact that the ball is not just a single point but rather a quad with a width and a height
	float ballWidth = (ball.width * ball.scale / 2);
	float ballHeight = (ball.height * ball.scale / 2);

	// Check for a collision between the ball and paddle1
	if ((ball.x - ballWidth) <= (paddle1.x + (paddle1.width * paddle1.scale / 2))) {	
		if (( (ball.y - ballHeight) <= (paddle1.y + (paddle1.height * paddle1.scale / 2))) && ( (ball.y + ballHeight) >= (paddle1.y - (paddle1.height * paddle1.scale / 2)))) {
			// This is a collision between ball and paddle1
			return 1;
		}
	}

	// Check for a collision between the ball and paddle2
	else if ( (ball.x + ballWidth) >= (paddle2.x - (paddle2.width * paddle2.scale / 2))) {	
		if (( (ball.y - ballHeight) <= (paddle2.y + (paddle2.height * paddle2.scale / 2))) && ( (ball.y + ballHeight) >= (paddle2.y - (paddle2.height * paddle2.scale / 2)))) {
			// This is a collision between ball and paddle2
			return 2;
		}
	}

	// Check for a collision between the ball and one of the walls
	else if ((ball.y + ballHeight) >= (topWall.y - (topWall.height * topWall.scale /2))) { 
		// This is a collisiion with the top wall
		return 3;
	}

	else if ((ball.y - ballHeight) <= (botWall.y - (botWall.height * botWall.scale / 2))) { 
		// This is a collisiion with the bottom wall
		return 3;
	}

	return 0;
}

/*	This function will complete the basic setup for the game. It should be run before the game loop.
	Setup SDL & OpenGL, Set projection matrix, Load Textures & Assign to Proper Entity
*/
void Setup(){

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse Assignment 2 - PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	// Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//--- Load Textures
	GLuint ballImg = LoadTexture("enemyUFO.png");
	GLuint paddle1Img = LoadTexture("laserBlue02.png");
	GLuint paddle2Img = LoadTexture("laserRed02.png");
	GLuint wallImg = LoadTexture("laserGreen03.png");
	GLuint winLogoImg = LoadTexture("winner.png");

	// Setting Entity textureIDs
	ball.textureID = ballImg;
	paddle1.textureID = paddle1Img;
	paddle2.textureID = paddle2Img;
	topWall.textureID = wallImg;
	botWall.textureID = wallImg;
	winLogo.textureID = winLogoImg;
}

/*	This function determines whether the game loop should continue.
	Returns a bool: true if the game should go on, false if the game should end
*/
bool ProcessEvents(){
	
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		// Check whether the close button for the game has been pressed -- if so, end the game
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		return true;
	}

	return true;
}

/*	This function sets the initial values for the Entities of this game:
	Ball, Paddles, Walls, Win Logo.
	Some settings can be changed for game balancing.
*/
void initEntityValues(){

	//--- Modifiable Settings for Balancing Game

	// Ball Settings
	ball.scale = 0.08f;
	ball.velocity_x = -1.0f;
	ball.velocity_y = 0.1f;
	ball.speed = 0.5f;

	// Paddle Settings (Changes are applied to both paddles)
	float paddle_speed = 2.0f;
	float paddle_height = 5.5f;
	float paddle_scale = 0.1f;
	float paddle_width = 0.5f;

	// Win Logo Settings
	winLogo.width = 2.5;

	//--- Modifications are NOT Recommended for the following

	// Paddle Settings
	paddle1.height = paddle_height;
	paddle1.width = paddle_width;
	paddle1.scale = paddle_scale;
	paddle1.speed = paddle_speed;
	paddle1.x = -1.10f;

	paddle2.height = paddle_height;
	paddle2.width = paddle_width;
	paddle2.scale = paddle_scale;
	paddle2.speed = paddle_speed;
	paddle2.x = 1.10f;

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

/*	This function will generate the sprites of the ball, paddles, and walls. 
	It requires the output of the function: winCheck()
	Depending on whether a winner has been decided, the game either continues OR the winner logo is displayed.
*/
void Render(bool winCheck){
	
	// Clear screen
	glClearColor(0.5f, 0.4f, 0.7f, 1.0f); // Purple Screen
	glClear(GL_COLOR_BUFFER_BIT);

	// If a winner has been decided, then display the win logo and stop displaying game elements
	if (winCheck) {
		winLogo.Draw();
	}

	// If a winner hasn't been decided yet, continue the game normally 
	else {
		ball.Draw();
		paddle1.Draw();
		paddle2.Draw();
		topWall.Draw();
		botWall.Draw();
	}

	// Display elements on the screen
	SDL_GL_SwapWindow(displayWindow);
}

/*	Handles player input to change the location of the paddles.
	Default Settings:	Paddle1 (left):		W for up, S for down
						Paddle2 (right):	UP for up, DOWN for down
*/
void Input(float elapsed) {
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
}

/*	Check if there is a winner
	Returns a bool: true if there is a winner, false if a winner hasn't been decided yet
*/
bool winCheck(){
	// If the ball has passed beyond the limits of the screen, a winner has been decided
	// While the current function doesn't record who the winner is, it can be easily modified to do so
	if ((ball.x > (1.40)) || (ball.x < (-1.40))) {
		return true;
	}
	return false;
}

/*	Process the ball's velocity and move the ball accordingly.
	Ball's acceleration and failSafe settings can be changed.
	Acceleration: How much the velocity of the ball increases with each "bounce" or collision with another entity
	failSafe: A constant float that will be used to slow the ball down should it get too fast due to acceleration
*/
void Update(float elapsed){

	// Modifiable Acceleration Constant (For game balancing)
	float accelConstant = 0.1f;
	float failSafe = 0.75f;

	// Process the ball movement and changes to the ball velocity
	// Ball colliding with paddle1
	if (ballCollisionCheck() == 1) { 
		
		ball.velocity_x *= -1.0f;					// Reverse the x velocity so that the ball starts going in the opposite direction
		ball.x += ball.velocity_x / 150.0f;			// Add in an offset so that the ball is not immediately touching the paddle again

		// Process the change to ball's x velocity
		if (fabs(ball.velocity_x) > 3.0) {
			// This is a fail safe mechanism to ensure that the ball doesn't get so fast that the game breaks
			// If the ball gets too fast the ball's speed is reduced based on a float that should be between 0.0f ~ 1.0f
			ball.velocity_x *= failSafe;
		}
		// We increase the x velocity of the ball if it's not going too fast 
		else if (ball.velocity_x > 0) {
			ball.velocity_x += accelConstant;
		}
		else {
			ball.velocity_x -= accelConstant;
		}

		// Process the change to ball's y velocity
		if (fabs(ball.velocity_y) > 3.0) {
			// This is a fail safe mechanism to ensure that the ball doesn't go too fast
			// The ball's y velocity is reduced according to the float failSafe 
			ball.velocity_y *= failSafe;
		}
		// Depending on the location of the paddle that the ball hits, the y velocity is changed
		else if (ball.y >= paddle1.y) { 
			ball.velocity_y += accelConstant;
		}
		else {
			ball.velocity_y -= accelConstant;
		}
	}
	// Ball colliding with paddle2 (most of the comments here are the same as for ballCollisionCheck() == 1 above)
	else if (ballCollisionCheck() == 2) { 
		
		ball.velocity_x *= -1.0f;					// Reverse the x velocity so that the ball starts going in the opposite direction
		ball.x += ball.velocity_x / 150.0f;			// Add in an offset so that the ball is not immediately touching the paddle again

		// Process the change to ball's x velocity
		if (fabs(ball.velocity_x) > 3.0) {
			// This is a fail safe mechanism to ensure that the ball doesn't get so fast that the game breaks
			// If the ball gets too fast the ball's speed is reduced based on a float that should be between 0.0f ~ 1.0f
			ball.velocity_x *= failSafe;
		}
		// We increase the x velocity of the ball if it's not going too fast 
		else if (ball.velocity_x > 0) {
			ball.velocity_x += accelConstant;
		}
		else {
			ball.velocity_x -= accelConstant;
		}

		// Process the change to ball's y velocity
		if (fabs(ball.velocity_y) > 3.0) {
			// This is a fail safe mechanism to ensure that the ball doesn't go too fast
			// The ball's y velocity is reduced according to the float failSafe 
			ball.velocity_y *= failSafe;
		}
		// Depending on the location of the paddle that the ball hits, the y velocity is changed
		else if (ball.y >= paddle1.y) { 
			ball.velocity_y += accelConstant;
		}
		else {
			ball.velocity_y -= accelConstant;
		}
	}
	// Ball collided with wall
	else if (ballCollisionCheck() == 3) { 
		ball.velocity_y *= -1.0f;					// Reverse the y velocity of the ball so that it will head in the opposite direction
		ball.y += ball.velocity_y / 90.0f;			// Add in an offset so the ball is not immediately touching the wall again

		// Depending on the direction of the ball, we increase the x velocity so that it should be going faster
		if (ball.velocity_x > 0) {
			ball.velocity_x += (1.25f * accelConstant);
		}
		else {
			ball.velocity_x -= (1.25f * accelConstant);
		}
	}

	// Based on the velocity, we want to move the ball's physical location
	ball.x += ball.velocity_x * elapsed;
	ball.y += ball.velocity_y * elapsed;
}

/*	Takes care of freeing things or closing things in order to close the game cleanly
	If controller support is added, each joystick needs to be closed in this function
*/
void CleanUp(){
	SDL_Quit();
}

/*	Sets up and runs the game of PONG
*/
int main(int argc, char *argv[]) {
	// Set up SDL / OpenGL / Projection Matrix / Textures before running the game loop
	Setup();	

	// Set up the initial values of the game's entities
	initEntityValues();

	// Time management variables
	float elapsed;
	float lastFrameTicks = 0.0f;

	// Game loop
	while (ProcessEvents()) {
		// Process time
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// Move the ball
		Update(elapsed);

		// Move the players' paddles
		Input(elapsed);

		// Render the game elements or the win logo depending on whether someone has won
		Render(winCheck());
	}

	// Clean up and close the game
	CleanUp();
	return 0;
}