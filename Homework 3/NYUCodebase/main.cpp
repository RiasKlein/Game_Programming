/*	Name: Shunman Tse
Fall 2014 - CS3113 Intro to Game Programming
Assignment 3: Create Space Invaders
*/

/*	Assignment Details:
- Make Space Invaders

Regarding this assignment:
- 3 States: Main Menu, Game Level, Game Over
- Keep and Display Score
- Use Sprite Sheets
*/

#include "SpaceInvadersApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	SpaceInvadersApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}

/* Credits: Include in the Game Over Menu
Space Invaders Logo - http://borys-kupar.github.io/space-invaders/images/logo.png
Player & Enemy Ships - By MillionthVector (http://millionthvector.blogspot.de)
*/