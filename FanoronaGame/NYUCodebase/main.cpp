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

Music:
Background Music: Pokemon Omega Ruby Alpha Sapphire OST

*/