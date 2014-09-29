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

/*	Notes about this version of Space Invaders:
	- Player 1 Controls: 
		A		to move LEFT
		D		to move RIGHT
		SPACE	to SHOOT
		ENTER	to CONTINUE
	- The enemies are not equipped with long range weaponry and therefore cannot fight back
	- High Scores are only kept for a single game session (once the application is closed, High Scores are reset)
	- High Scores are counted even if the player loses 
	- The player loses if the enemy ships make it to the bottom of the screen
*/

/*	LORE:
	An unknown alien force has attacked the colonies within the solar system, crippled the Earth Federation's fleets and are now heading toward Earth.
	Captain, you are piloting the last combat-ready ship that the Earth Federation has to offer.
	Do not let the remaining alien forces reach Earth.
*/

#include "SpaceInvadersApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	SpaceInvadersApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}

/* Credits: 
Space Invaders Logo - http://borys-kupar.github.io/space-invaders/images/logo.png
Player & Enemy Ships - By MillionthVector (http://millionthvector.blogspot.de)
Explosion Effects - By GameProgrammingSlave - http://opengameart.org/content/explosions
Lasers - By Rawdanitsu - http://opengameart.org/content/lasers-and-beams
Backgrounds - By Rawdanitsu - http://opengameart.org/content/space-backgrounds-2
*/