/*	Name: Shunman Tse
Fall 2014 - CS3113 Intro to Game Programming
Assignment 5 & 6: Scrolling Sidescroller
*/

/*	Assignment Details:
- Make a simple scrolling sidescroller or topdown game demo.
- It must use a tilemap.
- It must scroll.
- It must be either procedurally generated or load levels from a file.
	- This program will load level from a file
*/

/* About this game:
Controls:	Player 1:
			W	:	Jump	
			A	:	Go Left
			D	:	Go Right

Gameplay:	Reach the goal on the other side
*/

#include "PlatformerApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	PlatformerApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}
