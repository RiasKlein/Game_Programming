/*	Name: Shunman Tse
Fall 2014 - CS3113 Intro to Game Programming
Assignment 6: Add sound to an existing game
*/

/*	Assignment Details:
- Can use SDL_mixer or implement your own.
	- For this assignment I used SDL_mixer
- Must have music and at least two sound effects.
*/

/* About this game:
Controls:	Player 1:
			W	:	Jump	
			A	:	Go Left
			D	:	Go Right
			S	:	All Stop (no velocity / no x acceleration)

			Player 2:
			UP	:	Jump
			LEFT:	Go Left
			RIGHT:	Go Right
			DOWN:	All Stop

Gameplay:	Two players will compete to collect the most points

Features:	1. Endless spawn of coins -- players can keep going until they are both satisfied with the results
			2. Score counter for each player -- lets players know who's in the lead
			3. Player Killing (PK) -- Jump on the head of the enemy player to force them to the respawn point 
			   (Beware, the enemy may fight back)
*/

#include "PlatformerApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	PlatformerApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}
