/*	Name: Shunman Tse
	Fall 2014 - CS3113 Intro to Game Programming
	Assignment 8: Adding Effects
*/

/* About this game:
	UP :	Move forward
	LEFT :	Rotate left
	RIGHT : Rotate right
	DOWN :	All stop

	SPACE : Shoot
*/

/*	Effects
	1. Player engines
	2. Crashing into asteroids
	3. Destroying asteroids
*/

#include "AsteroidsApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	AsteroidsApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}
