#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include "Entity.h"
#include "Chip.h"
#include "MovingText.h"
//#include <cmath>
//#include <vector>
//#include "SheetSprite.h"

using namespace std;

//#define FIXED_TIMESTEP 0.0166666f	// 60 FPS
//#define FIXED_TIMESTEP 0.0333333f // 30 FPS

class Fanorona {
public:
	Fanorona();
	~Fanorona();

	//	Function for setting up players and walls
	void Init();

	//	Functions for visual display and for managing the flow of the game
	void Render();
	void Update(float elapsed);
	bool UpdateAndRender();

	//	Functions for reading user input
	int MouseToBoardRow(float mouseY);
	int MouseToBoardCol(float mouseX);

	//	Functions for processing the game logic
	void clearChipsMoves(vector<Chip*> currentchips);
	void populateChipMoves(Chip* chip, vector<Chip*> currentchips, bool is3x3board);
	bool paikaCheck();
	bool validMoveCheck();
	void processCapture();
	void processPaika();
	void endTurn();

	//	Functions related to animations 
	void populatePossibleAttacks(Chip* chip);

	//	Function for printing text to the screen
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);

	//	Lerp Function
	float lerp(float v0, float v1, float t);

private:
	bool done = false;
	float lastFrameTicks;
	float timeLeftOver = 0.0f;
	SDL_Window* displayWindow;

	//	Textures
	GLuint font;							//	Font texture
	GLuint title_texture;					//	Title page texture
	GLuint ui_texture;						//	Texture for the UI
	GLuint boards_logo_texture;				//	Texture with the different boards and the logo : 602 * 602
	GLuint players_texture;					//	Texture with the player chips : 138 * 138
	GLuint flags_texture;					//	Texture for the flag animations : 416 * 416
	GLuint movingtext_texture;				//	Texture for moving text objects : 882 * 882

	//	Music & Sounds
	vector<Mix_Music*> gameMusic;			//	Index 0 is the title music, everything else are variations of game music
	int volume = 100;						//	Volume of the music (increments of 25, max of 100)
	int tracknumber = 0;					//	Track number to play in game
	bool updatemusic = false;				//	Whether there is a need to update the music channel
	bool updatevolume = false;				//	Whether there is a need to update the volume of the channel

	Mix_Chunk* chipMove_sound;
	Mix_Chunk* button_sound;

	Entity* board;							//	Image Entity of the current board being played on
	vector<Entity*> mainmenu;				//	Vector of Entities for the main menu
	vector<Chip*> currentchips;				//	Vector of chips on the current board

	//	Related to ANIMATIONS
	vector<Chip*> possibleATTACKS;			//	Vector of sword animations to show chips that can be attacked
	vector<Entity*> animatedBflags;			//	Entity of black flag animations	
	vector<Entity*> animatedWflags;			//	Entity of white flag animations
	vector<Chip*> possiblePAIKA;			//	This is just for the 3x3 board to show where people can PAIKA

	//	Related to the ESC Menu
	vector <MovingText*> escMenu;			//	This contains the objects for the esc menu

	//	Variables to handle a player turn
	bool paikaEnabled = false;				//	Whether paika is allowed for the current turn
	bool validPiece = false;				//	Whether a valid piece has been chosen by the current team
	int validPieceCol = 0;					//	Column variable for the selected piece to move
	int validPieceRow = 0;					//	Row variable for the selected piece to move
	int targetCol = 0;						//	Column variable for the target piece
	int targetRow = 0;						//	Row variable for the target piece
	enum AttackType {
		ATK_NULL, ATK_WITHDRAWUP, ATK_WITHDRAWDOWN, ATK_WITHDRAWLEFT, ATK_WITHDRAWRIGHT, ATK_WITHDRAWULEFT, ATK_WITHDRAWURIGHT, ATK_WITHDRAWBLEFT, ATK_WITHDRAWBRIGHT,
		ATK_APPROACHUP, ATK_APPROACHDOWN, ATK_APPROACHLEFT, ATK_APPROACHRIGHT, ATK_APPROACHULEFT, ATK_APPROACHURIGHT, ATK_APPROACHBLEFT, ATK_APPROACHBRIGHT, ATK_PAIKA
	};
	int attacktype = ATK_NULL;				//	attacktype is used to store the move type that the player is aiming for

	//	Game Settings
	bool is3x3board = true;					//	Whether the board being played on is 3x3 or not
	bool is7x7board = false;				//	Whether the board being played on is 7x7 or not
	bool playerisWhite = true;				//	Whether the player is white or not

	//	Game states
	enum GameState { STATE_MAINMENU, STATE_WHITEPLAYER, STATE_BLACKPLAYER, STATE_GAMEOVER, STATE_GAMERULES1, STATE_GAMERULES2 };
	int gamestate = STATE_MAINMENU;			// The initial game state is the main menu

	enum GameMenu { MENU_OFF, MENU_ON };
	int gamemenu = MENU_OFF;

	//	Gameover states
	enum GamOver { GAMEOVER_NULL, GAMEOVER_WHITEWINS, GAMEOVER_BLACKWINS };
	int gameover = GAMEOVER_NULL;

	//	Moving Text Messages
	enum ScreenText { SCREENTEXT_WHITETURN, SCREENTEXT_BLACKTURN, SCREENTEXT_WHITEWINS, SCREENTEXT_BLACKWINS };
	int textIndex = SCREENTEXT_WHITETURN;
	vector<MovingText*> screenText;

	//	Tracking Mouse Motion
	float mouseX = 0.0f;					// X position of most recent mouse click (from -2 ~ 2)
	float mouseY = 0.0f;					// Y position of most recent mouse click (from -2 ~ 2)

	int turnCounter = 0;

	//	LOGGER Messages (Used for debugging)
	bool LOG_MASTER = false;				// Enables the debug log entries -- if this is false, no log entries will print
	bool LOG_TRACEMOUSEDOWN = true;			// Console log: entry for mouse click information
	bool LOG_MUSIC = true;					// Console log: entry for changes to music settings
	bool LOG_CHIPSSIZE = true;				// Console log: entry for the size of the currentchips vector
	bool LOG_CLICKEDCHIP = true;			// Console log: entry for the chip for that was clicked
	bool LOG_POPULATECHIPMOVES = true;		// Console log: entry for the results of populateChipMoves
	bool LOG_MENUS = true;					// Console log: entry for the menu status
};