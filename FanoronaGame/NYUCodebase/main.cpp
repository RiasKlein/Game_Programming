/*	
Name: Shunman Tse
Fall 2014 - CS6613 Artificial Intelligence
Final Project - Project Fanorona
*/

/*	Assignment Details:

*/

/*	Game Mechanics:

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