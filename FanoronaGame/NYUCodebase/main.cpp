/*	
	Name: Shunman Tse
	Fall 2014 - CS3113 Intro To Game Programming
	Fanorona Game
*/

/*	Assignment Details:
	- Must have a title screen and proper states for game over, etc.
	- Must have a way to quit the game.
	- Must have music and sound effects.
	- Must have at least 3 different levels or be procedurally generated.
	- Must be either local multiplayer or have AI (or both!).
	- Must have at least some animation or particle effects.
*/

/*	Gameplay:
	- 2 Player Local Multiplayer
	- Both players share the same mouse
	- White player goes first ALWAYS!

	- ESC may be pressed for the menu during gameplay

	- 3x3 Board comes with guidance for PAIKA and CAPTURE moves
	- 5x5 Board comes with just CAPTURE guidance
	- 7x7 Board comes with no guidance
*/

/*	Fanorona Game Rules:
	- You must capture if there is an available move to do so
		- There are two types are captures:
			1. Approach capture: You move to be beside an enemy piece and that enemy piece is captured
			2. Withdraw capture: You move away from an enemy that is next to you and that enemy piece is captured
	- If no capture moves are availble, the player may do a PAIKA move which means the player may move freely to an adjacent position
	- Captures in a certain direction will continue until either the border of the board is met or a friendly piece is met
		- For example, an attack in the UR direction will capture all adjacent enemy chips in the UR direction (it can be broken when the attack reaches a friendly chip)
*/

#include "Fanorona.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	Fanorona app;
	while (!app.UpdateAndRender()) {}
	return 0;
}

/* Credits

Images:
Title Background: http://pichost.me/1867826/
Wooden Board Texture (modified): http://homeornaments.com/wooden-floor/
Black / White chips (modified): http://opengameart.org/content/boardgame-pack
Logo (generated on free logo website): http://www.flamingtext.com/
Flags (modified) : http://www.spriters-resource.com/pc_computer/captainclaw/sheet/47607/

Music:
Background Music: Pokemon Omega Ruby Alpha Sapphire OST

Sound Effect:
Chip Sound Effect: Button 16 at http://www.soundjay.com/button-sounds-2.html
*/