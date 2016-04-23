/*	Name: Shunman Tse
	Fall 2014 - CS3113 Intro to Game Programming
	Assignment 7: Asteroids
*/

/*	Assignment Details:
	- Make simple asteroids
	- You must use transformation matrices and non-axis-aligned collisions!
*/

/* About this game:
	UP :	Move forward
	LEFT :	Rotate left
	RIGHT : Rotate right
	DOWN :	All stop

	SPACE : Shoot
*/

#include "AsteroidsApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	AsteroidsApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}
