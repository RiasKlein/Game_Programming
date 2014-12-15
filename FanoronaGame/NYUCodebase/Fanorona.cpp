#include "Fanorona.h"

/*	Use SDL to load in textures. The function returns GLuint textureID.
*/
GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

float Fanorona::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

/*	This function sets up the basics for graphical displays on the screen.
*/
Fanorona::Fanorona() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Shunman Tse - CS3113 Fanorona", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	//--- Basic setup for the rendering pipeline
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//	Print out Project Fanorona Details
	cout << "============================================================\n\tProject Fanorona - Fall 2014 by Shunman Tse\n============================================================\n" << endl;

	//	Print out in Console which Logging Tools are active
	if (LOG_MASTER)			{ cout << "Logger Tools\t\t\t...\t\tActive" << endl; }
	else					{ cout << "Logger Tools\t\t\t...\t\tInactive" << endl; }
	if (LOG_TRACEMOUSEDOWN)	{ cout << "Mouse Click Tracing\t\t...\t\tActive" << endl; }
	else					{ cout << "Mouse Click Tracing\t\t...\t\tInactive" << endl; }
	if (LOG_MUSIC)			{ cout << "Music Tracing\t\t\t...\t\tActive" << endl; }
	else					{ cout << "Music Tracing\t\t\t...\t\tInactive" << endl; }
	if (LOG_CHIPSSIZE)		{ cout << "Chip Size Tracing\t\t...\t\tActive" << endl; }
	else					{ cout << "Chip Size Tracing\t\t...\t\tInactive" << endl; }
	if (LOG_CLICKEDCHIP)	{ cout << "Chip Click Tracing\t\t...\t\tActive" << endl; }
	else					{ cout << "Chip Click Tracing\t\t...\t\tInactive" << endl; }
	cout << endl;

	Init();
}

/*	This function is run once at the start of the game.
	It loads in textures, music, sounds, and generates the Main Menu.
*/
void Fanorona::Init() {
	lastFrameTicks = 0.0f;

	//======================================================================================================================
	//--- Load in textures
	//======================================================================================================================

	font = LoadTexture("font.png");									//	512 * 512
	title_texture = LoadTexture("title.png");						//	1920 * 1080
	ui_texture = LoadTexture("ui.png");								//	512 * 512
	boards_logo_texture = LoadTexture("boards&logo.png");			//	602 * 602
	players_texture = LoadTexture("players.png");					//	138 * 138
	flags_texture = LoadTexture("BWFlags.png");						//	416 * 416
	movingtext_texture = LoadTexture("movingtext.png");				//	882 * 882

	//======================================================================================================================
	//--- Load in Music / Sounds
	//======================================================================================================================

	chipMove_sound = Mix_LoadWAV("chipMoveSound.wav");
	button_sound = Mix_LoadWAV("buttonSound.wav");

	gameMusic.push_back(Mix_LoadMUS("title.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track01.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track02.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track03.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track04.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track05.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track06.mp3"));
	gameMusic.push_back(Mix_LoadMUS("track07.mp3"));

	Mix_VolumeMusic(volume);
	Mix_PlayMusic(gameMusic[tracknumber], -1);

	//======================================================================================================================
	//--- Set up the main menu
	//		The following code creates Entity objects that are part of the main menu and gives each of them proper images.
	//		The finalized Entity objects are pushed onto the main menu vector which can be rendered to display the main menu.
	//======================================================================================================================
	SheetSprite bg_sprite = SheetSprite(title_texture, 0.0f, 0.0f, 1.0f, 1.0f);
	Entity* bg = new Entity(bg_sprite, -0.8f, 0.0f, 2.9f);
	mainmenu.push_back(bg);

	SheetSprite logo_sprite = SheetSprite(boards_logo_texture, 301.0f / 602.0f, 301.0f / 602.0f, 300.0f / 602.0f, 111.0f / 602.0f);
	Entity* logo = new Entity(logo_sprite, -0.85f, 1.0f, 2.2f);
	mainmenu.push_back(logo);

	// Board Setting : mainmenu[2]
	SheetSprite boardsettingsprite = SheetSprite(ui_texture, 0.0f, 188.0f / 512.0f, 190.0f / 512.0f, 49.0f / 512.0f);
	Entity* boardsetting = new Entity(boardsettingsprite, 1.1f, 1.0f, 2.0f);
	mainmenu.push_back(boardsetting);

	// Rulebook : mainmenu[3]
	Entity* playersetting = new Entity(boardsettingsprite, 1.1f, 0.45f, 2.0f);
	mainmenu.push_back(playersetting);
	//===*/

	SheetSprite left_sprite = SheetSprite(ui_texture, 193.0f / 512.0f, 486.0f / 512.0f, 22.0f / 512.0f, 21.0f / 512.0f);
	SheetSprite right_sprite = SheetSprite(ui_texture, 215.0f / 512.0f, 486.0f / 512.0f, 22.0f / 512.0f, 21.0f / 512.0f);

	// Track Setting (-) : mainmenu[4]
	Entity* tracksettingLEFT = new Entity(left_sprite, 0.45f, -0.5f, 2.0f);
	mainmenu.push_back(tracksettingLEFT);

	// Track Setting (+) : mainmenu[5]
	Entity* tracksettingRIGHT = new Entity(right_sprite, 1.75f, -0.5f, 2.0f);
	mainmenu.push_back(tracksettingRIGHT);

	// Volume Setting (-) : mainmenu[6]
	Entity* volumesettingLEFT = new Entity(left_sprite, 1.0f, -0.90f, 2.0f);
	mainmenu.push_back(volumesettingLEFT);

	// Volume Setting (+) : mainmenu[7]
	Entity* volumesettingRIGHT = new Entity(right_sprite, 1.75f, -0.90f, 2.0f);
	mainmenu.push_back(volumesettingRIGHT);

	// Start Button : mainmenu[8]
	Entity* startbutton = new Entity(boardsettingsprite, 1.1f, -1.62f, 2.0f);
	mainmenu.push_back(startbutton);

	// Next Page Button -> : mainmenu[9]
	Entity* nextPageRight = new Entity(right_sprite, 1.5f, -1.5f, 5.0f);
	mainmenu.push_back(nextPageRight);

	// Next Page Button <- : mainmenu[10]
	Entity* nextPageLeft = new Entity(left_sprite, -1.5f, -1.5f, 5.0f);
	mainmenu.push_back(nextPageLeft);

	//======================================================================================================================
	//--- Set up FLAG animations
	//		The flag animations will be used in the game over state for the winning team
	//======================================================================================================================
	float flagAnimationTime = 1.0f;		// The number of seconds for the flags to finish their animations

	//	White Flag
	SheetSprite whiteflag05 = SheetSprite(flags_texture, 218.0f / 416.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag06 = SheetSprite(flags_texture, 271.0f / 416.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag07 = SheetSprite(flags_texture, 218.0f / 416.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag08 = SheetSprite(flags_texture, 164.0f / 416.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag09 = SheetSprite(flags_texture, 110.0f / 416.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag10 = SheetSprite(flags_texture, 0.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag11 = SheetSprite(flags_texture, 55.0f / 416.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag12 = SheetSprite(flags_texture, 56.0f / 416.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite whiteflag13 = SheetSprite(flags_texture, 56.0f / 416.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);

	Entity* leftWhiteFlag = new Entity(whiteflag05, -1.4f, -0.5f, 2.0f);
	Entity* rightWhiteFlag = new Entity(whiteflag05, 1.4f, -0.5f, 2.0f);
	animatedWflags.push_back(leftWhiteFlag);
	animatedWflags.push_back(rightWhiteFlag);
	
	for (unsigned int i = 0; i < animatedWflags.size(); i++) {
		animatedWflags[i]->timeToComplete = flagAnimationTime;
		animatedWflags[i]->animationFrames.push_back(whiteflag06);
		animatedWflags[i]->animationFrames.push_back(whiteflag07);
		animatedWflags[i]->animationFrames.push_back(whiteflag08);
		animatedWflags[i]->animationFrames.push_back(whiteflag09);
		animatedWflags[i]->animationFrames.push_back(whiteflag10);
		animatedWflags[i]->animationFrames.push_back(whiteflag11);
		animatedWflags[i]->animationFrames.push_back(whiteflag12);
		animatedWflags[i]->animationFrames.push_back(whiteflag13);
		animatedWflags[i]->animationFrames.push_back(whiteflag05);
	}

	//	Black Flag
	SheetSprite blackflag05 = SheetSprite(flags_texture, 271.0f / 416.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag06 = SheetSprite(flags_texture, 270.0f / 416.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag07 = SheetSprite(flags_texture, 218.0f / 416.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag08 = SheetSprite(flags_texture, 164.0f / 416.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag09 = SheetSprite(flags_texture, 110.0f / 416.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag10 = SheetSprite(flags_texture, 0.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag11 = SheetSprite(flags_texture, 0.0f, 226.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag12 = SheetSprite(flags_texture, 164.0f / 416.0f, 113.0f / 416.0f, 53.0f / 416.0f, 112.0f / 416.0f);
	SheetSprite blackflag13 = SheetSprite(flags_texture, 110.0f / 416.0f, 0.0f, 53.0f / 416.0f, 112.0f / 416.0f);

	Entity* leftBlackFlag = new Entity(blackflag05, -1.4f, -0.5f, 2.0f);
	Entity* rightBlackFlag = new Entity(blackflag05, 1.4f, -0.5f, 2.0f);
	animatedBflags.push_back(leftBlackFlag);
	animatedBflags.push_back(rightBlackFlag);

	for (unsigned int i = 0; i < animatedBflags.size(); i++) {
		animatedBflags[i]->timeToComplete = flagAnimationTime;
		animatedBflags[i]->animationFrames.push_back(blackflag06);
		animatedBflags[i]->animationFrames.push_back(blackflag07);
		animatedBflags[i]->animationFrames.push_back(blackflag08);
		animatedBflags[i]->animationFrames.push_back(blackflag09);
		animatedBflags[i]->animationFrames.push_back(blackflag10);
		animatedBflags[i]->animationFrames.push_back(blackflag11);
		animatedBflags[i]->animationFrames.push_back(blackflag12);
		animatedBflags[i]->animationFrames.push_back(blackflag13);
		animatedBflags[i]->animationFrames.push_back(blackflag05);
	}

	//======================================================================================================================
	//--- Set up Moving Text Objects
	//		The flag animations will be used in the game over state for the winning team
	//======================================================================================================================

}

/*	~Fanorona()
	Called when the game should be closed.
*/
Fanorona::~Fanorona() {
	//======================================================================================================================
	//--- Free the Music / Sound Assets that were loaded in
	//		In the following code, the music tracks that were loaded in for the game are freed. 
	//======================================================================================================================
	for (unsigned int i = 0; i < gameMusic.size(); i++){
		Mix_FreeMusic(gameMusic[i]);
	}

	Mix_FreeChunk(chipMove_sound);
	Mix_FreeChunk(button_sound);

	//======================================================================================================================
	//--- Close the game
	//======================================================================================================================
	SDL_Quit();
}

/*	This function renders the game objects onto the screen based on the state of the game
	There are four states to consider: the main menu, the white player's turn, the black player's turn and game over
*/
void Fanorona::Render() {
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // Default Clear Screen
	glClear(GL_COLOR_BUFFER_BIT);

	//	Render the game based on the GameState
	if (gamestate == STATE_MAINMENU){
		//==================================================================================================================
		//--- Render the Main Menu
		//		First the Entity objects of the main menu are rendered onto the screen
		//		Then the text to accompany those Entities are rendered onto the screen
		//==================================================================================================================
		for (unsigned int i = 0; i < 9; i++){
			mainmenu[i]->Render();
		}

		// Text : "Game Settings"
		glLoadIdentity();
		glTranslatef(0.35f, 1.5f, 0.0f);
		Text(font, "Game Settings", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

		// Text : "Music Settings"
		glLoadIdentity();
		glTranslatef(0.28f, -0.15f, 0.0f);
		//glTranslatef(0.0f, -1.65f, 0.0f);
		Text(font, "Music Settings", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

		// Text : "Volume"
		glLoadIdentity();
		glTranslatef(0.28f, -0.88f, 0.0f);
		//glTranslatef(0.0f, -0.83f, 0.0f);
		Text(font, "Volume", 0.19f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

		// Text : Volume Setting
		glLoadIdentity();
		glTranslatef(1.26f, -0.88f, 0.0f);
		//glTranslatef(0.98f, 0.0f, 0.0f);
		Text(font, to_string(volume), 0.19f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

		// Text : Track Setting
		glLoadIdentity();
		glTranslatef(0.81f, -0.5f, 0.0f);
		//glTranslatef(-0.45f, 0.48f, 0.0f);
		Text(font, "Track " + to_string(tracknumber), 0.19f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

		// Text : Board Setting -- Tell whether we have a 3x3 board or a 5x5 board
		glLoadIdentity();
		glTranslatef(0.6f, 1.03f, 0.0f);
		if (is3x3board) {
			Text(font, "3x3 Board", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);
		}
		else if (!is3x3board && !is7x7board){
			Text(font, "5x5 Board", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);
		}
		else {
			Text(font, "7x7 Board", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);
		}

		//=== Text : Game Rules
		glLoadIdentity();
		glTranslatef(0.55f, 0.48f, 0.0f);
		Text(font, "Game Rules", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);
		//===*/

		// Text : Start Button
		glLoadIdentity();
		glTranslatef(0.85f, -1.6f, 0.0f);
		Text(font, "Start", 0.22f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);
	} // above is Render() : gamestate == STATE_MAINMENU 

	else if (gamestate == STATE_WHITEPLAYER || gamestate == STATE_BLACKPLAYER) {
		//==================================================================================================================
		//--- Render Board for Player Turns
		//		Render the board and chips on the board to the screen
		//==================================================================================================================

		//	Render a background just in case and then render the board on top
		mainmenu[0]->Render();
		board->Render();

		//	Render the chips on the board
		for (unsigned int i = 0; i < currentchips.size(); i++){
			currentchips[i]->Render();
		}

		//	Render the attack icons on the board
		for (unsigned int i = 0; i < possibleATTACKS.size(); i++) {
			possibleATTACKS[i]->Render();
		}

	} // above is Render() : gamestate == STATE_WHITEPLAYER || gamestate == STATE_BLACKPLAYER
	
	else if (gamestate == STATE_GAMEOVER) {
		//	Render the board and chips
		board->Render();
		for (unsigned int i = 0; i < currentchips.size(); i++){
			currentchips[i]->Render();
		}
		mainmenu[8]->Render();	//	Render a button where the start button would've been in the main menu

		//	Print out who won!
		if (gameover == GAMEOVER_WHITEWINS) { // White won!
			glLoadIdentity();
			glTranslatef(-1.75f, +0.5f, 0.0f);
			Text(font, "WHITE WINS!", 0.45f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

			//	Show the animated white flag!
			for (unsigned int i = 0; i < animatedWflags.size(); i++) {
				animatedWflags[i]->Render();
			}
		}
		else if (gameover == GAMEOVER_BLACKWINS) { // Black won!
			glLoadIdentity();
			glTranslatef(-1.75f, +0.5f, 0.0f);
			Text(font, "BLACK WINS!", 0.45f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

			//	Show the animated black flag!
			for (unsigned int i = 0; i < animatedBflags.size(); i++) {
				animatedBflags[i]->Render();
			}
		}

		//	The button text will be "Go to Main Menu"
		glLoadIdentity();
		glTranslatef(0.47f, -1.6f, 0.0f);
		Text(font, "Go to Main Menu", 0.185f, -0.095f, 191.0f / 255.0f, 127.0f / 255.0f, 63.0f / 255.0f, 1.0f);

	} // above is Render() : gamestate == STATE_GAMEOVER 

	else if (gamestate == STATE_GAMERULES1) {
		//	Render the right button (for changing to STATE_GAMERULES2)
		mainmenu[0]->Render();
		mainmenu[9]->Render();

		//	Render the text for the rulebook
		glLoadIdentity();
		glTranslatef(-0.2f, 1.8f, 0.0f);
		Text(font, "Rules", 0.22f, -0.095f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, 1.5f, 0.0f);
		Text(font, "1.You must capture enemies if you can.", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, 1.2f, 0.0f);
		Text(font, "2.There are two types of capture moves:", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, 0.9f, 0.0f);
		Text(font, "Approach Capture:", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.4f, 0.6f, 0.0f);
		Text(font, "Move toward the enemy", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, 0.3f, 0.0f);
		Text(font, "Withdraw Capture:", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.4f, 0.0f, 0.0f);
		Text(font, "Move away from the enemy", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, -0.3f, 0.0f);
		Text(font, "3.A piece may move freely if no", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, -0.6f, 0.0f);
		Text(font, "friendly pieces can capture. This", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, -0.9f, 0.0f);
		Text(font, "is called a \"Paika\" move.", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, -1.2f, 0.0f);
		Text(font, "4. White ALWAYS goes first.", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		// Button Text
		glLoadIdentity();
		glTranslatef(1.2f, -1.8f, 0.0f);
		Text(font, "Next Page", 0.18f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

	} // above is Render() : gamestate == STATE_GAMERULES1

	else if (gamestate == STATE_GAMERULES2) {
		//	Render the left and right buttons
		mainmenu[0]->Render();
		mainmenu[9]->Render();
		mainmenu[10]->Render();

		glLoadIdentity();
		glTranslatef(-1.0f, 1.8f, 0.0f);
		Text(font, "Playing the Game", 0.22f, -0.095f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, 1.5f, 0.0f);
		Text(font, "1.Use the mouse to click on a friendly", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, 1.2f, 0.0f);
		Text(font, "piece to move", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.9f, 0.9f, 0.0f);
		Text(font, "2.Click on the enemy piece to capture", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.65f, 0.6f, 0.0f);
		Text(font, "Or click the position to Paika to", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.5f, -0.3f, 0.0f);
		Text(font, "This game is LOCAL MULTIPLAYER", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-1.85f, -0.6f, 0.0f);
		Text(font, "Both players must share the same mouse", 0.20f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		// Text for Buttons
		glLoadIdentity();
		glTranslatef(-1.8f, -1.8f, 0.0f);
		Text(font, "Previous Page", 0.18f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(1.2f, -1.8f, 0.0f);
		Text(font, "Main Menu", 0.18f, -0.0999f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	SDL_GL_SwapWindow(displayWindow);
}

/*	Update function
	
*/
void Fanorona::Update(float elapsed) {
	//======================================================================================================================
	//--- Update Music / Volume (if needed)
	//		The following code will update the music track and the volume if a change was made
	//======================================================================================================================
	if (updatemusic) {
		Mix_PlayMusic(gameMusic[tracknumber], -1);
		updatemusic = false;
	}
	if (updatevolume){
		Mix_VolumeMusic(volume);
		updatevolume = false;
	}

	//	Update Animations
	for (unsigned int i = 0; i < possibleATTACKS.size(); i++) {
		possibleATTACKS[i]->Update(elapsed);
	}

	//	Only update flag animations if it's the gameover state
	if (gamestate == STATE_GAMEOVER){
		//	Update both flags it doesn't matter since just one will be shown on the screen
		for (unsigned int i = 0; i < animatedBflags.size(); i++) {
			animatedBflags[i]->Update(elapsed);
			animatedWflags[i]->Update(elapsed);
		}
	}
}

/*	Game loop
	Handle player inputs here
*/
bool Fanorona::UpdateAndRender() {

	//	Updating the timing of the game
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		//	Check whether the close button for the game has been pressed -- if so, end the game
		//	The shortcut to close the game is hit "ESC" on the keyboard
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		//	Pressing ESC should open up a menu only if it's during one of the gameplay states
		else if (event.button.button == SDL_SCANCODE_ESCAPE) {
			if ((gamestate == STATE_WHITEPLAYER || gamestate == STATE_BLACKPLAYER) && (gamemenu == MENU_OFF)) {
				//	Set gamemenu to be ON
				gamemenu = MENU_ON;

				//	Display menu elements

				if (LOG_MENUS && LOG_MASTER) {
					cout << "LOG Menu:\tON" << endl;
				}
//=================================================================

			}
		}
		//	Check whether there has been mouse movement -- if there was movement then update the position variables
		else if (event.type == SDL_MOUSEMOTION) {
			mouseX = (((float)event.motion.x / 800.0f) * 4.0f) - 2.0f;
			mouseY = (((float)(600 - event.motion.y) / 600.0f) * 4.0f) - 2.0f;
		}
		//	If the mouse was pressed down, the position of the mouse as traced by the code above can be used to locate where the mouse was pressed down
		//	Knowing where the mouse press occured, the game can be updated accordingly
		else if (event.type == SDL_MOUSEBUTTONDOWN){

			if (LOG_TRACEMOUSEDOWN && LOG_MASTER) {
				//	Console Trace for mouse down positions
				cout << fixed << setprecision(3) << "LOG Mouse Click:\tX Position: " << mouseX << "\tY Position: " << mouseY << endl;
			}

			// Depending on the gamestate, treat the mouse click differently
			if (gamestate == STATE_MAINMENU) { // Mouse was clicked in the main menu
				// Find out where the mouse click occured and process it

				// Board Settings : 3x3 -> 5x5 -> 7x7
				if (mouseX >= 0.360 && mouseX <= 1.835 && mouseY >= 0.827 && mouseY <= 1.187) {
					Mix_PlayChannel(-1, button_sound, 0);

					if (is3x3board) {
						is3x3board = false;
						is7x7board = false;
					}
					else if (!is3x3board && !is7x7board) { // It's not 3x3 and not 7x7 -> must be 5x5
						is3x3board = false;
						is7x7board = true;
					}
					else { // This is the 7x7 board
						is7x7board = false;
						is3x3board = true;
					}

					//	Console trace for game board change
					if (LOG_MASTER && LOG_TRACEMOUSEDOWN) {
						if (is3x3board) {
							cout << "LOG Game board is:\t3x3" << endl;
						}
						else if (!is3x3board && !is7x7board){
							cout << "LOG Game board is:\t5x5" << endl;
						}
						else {
							cout << "LOG Game board is:\t7x7" << endl;
						}
					}
				}

				//=== Game Rules Button
				if (mouseX >= 0.360 && mouseX <= 1.835 && mouseY >= 0.293 && mouseY <= 0.640) {
					Mix_PlayChannel(-1, button_sound, 0);

					gamestate = STATE_GAMERULES1;

					//	Console trace for player team change
					if (LOG_MASTER && LOG_TRACEMOUSEDOWN) {
						cout << "LOG Game Rules:\t\tClicked" << endl;
					}
				}
				//===*/

				// Music Settings : Track (-)
				if (mouseX >= 0.360 && mouseX <= 0.530 && mouseY >= -0.573 && mouseY <= -0.420) {
					Mix_PlayChannel(-1, button_sound, 0);

					if (tracknumber > 0) {
						tracknumber -= 1;
						updatemusic = true;

						//	Console trace for music changes
						if (LOG_MUSIC && LOG_MASTER) {
							cout << "LOG Music Track:\t" << tracknumber << endl;
						}
					}
				}

				// Music Settings : Track (+)
				if (mouseX >= 1.670 && mouseX <= 1.835 && mouseY >= -0.573 && mouseY <= -0.420) {
					Mix_PlayChannel(-1, button_sound, 0);

					if (tracknumber < (gameMusic.size() - 1)) {
						tracknumber += 1;
						updatemusic = true;

						//	Console trace for music changes
						if (LOG_MUSIC && LOG_MASTER) {
							cout << "LOG Music Track:\t" << tracknumber << endl;
						}
					}
				}

				// Volume Settings : Volume (-)
				if (mouseX >= 0.915 && mouseX <= 1.080 && mouseY >= -0.973 && mouseY <= -0.820) {
					Mix_PlayChannel(-1, button_sound, 0);

					if (volume > 0) {
						volume -= 25;
						updatevolume = true;

						// Console trace for volume change
						if (LOG_MUSIC && LOG_MASTER) {
							cout << "LOG Volume:\t\t" << volume << endl;
						}
					}				
				}

				// Volume Settings : Volume (+)
				if (mouseX >= 1.655 && mouseX <= 1.820 && mouseY >= -0.973 && mouseY <= -0.820) {
					Mix_PlayChannel(-1, button_sound, 0);

					if (volume < 100) {
						volume += 25;
						updatevolume = true;

						// Console trace for volume change
						if (LOG_MUSIC && LOG_MASTER) {
							cout << "LOG Volume:\t\t" << volume << endl;
						}
					}
				}

				// Start Game Button 
				if (mouseX >= 0.360 && mouseX <= 1.835 && mouseY >= -1.793 && mouseY <= -1.420) {
					Mix_PlayChannel(-1, button_sound, 0);

					//	Set up the board based on the variable : is3x3board
					if (is3x3board) {
						SheetSprite boardsprite = SheetSprite(boards_logo_texture, 0.0f, 0.0f, 300.0f/602.0f, 300.0f/602.0f);
						board = new Entity(boardsprite, 0.0f, 0.0f, 4.0f);
					}
					else if (!is3x3board && !is7x7board){ // 5x5 Board
						SheetSprite boardsprite = SheetSprite(boards_logo_texture, 0.0f, 301.0f / 602.0f, 300.0f / 602.0f, 300.0f / 602.0f);
						board = new Entity(boardsprite, 0.0f, 0.0f, 4.0f);
					}
					else { // 7x7 board
						SheetSprite boardsprite = SheetSprite(boards_logo_texture, 301.0f / 602.0f, 0.0f, 300.0f / 602.0f, 300.0f / 602.0f);
						board = new Entity(boardsprite, 0.0f, 0.0f, 4.0f);
					}

					//	Populate vector of chips on the board
					SheetSprite whitesprite = SheetSprite(players_texture, 69.0f / 138.0f, 0.0f, 68.0f / 138.0f, 68.0f / 138.0f);
					SheetSprite blacksprite = SheetSprite(players_texture, 0.0, 0.0, 68.0 / 138.0f, 68.0 / 138.0f);

					if (is3x3board) {
						// Clear the currentchips vector just in case there was something inside
						currentchips.clear();

						for (unsigned int i = 1; i <= 3; i++) { // Top row of all black chips
							Chip* chip = new Chip(blacksprite, i, 1, is3x3board, is7x7board, false);
							currentchips.push_back(chip);
						}

						Chip* chip1 = new Chip(blacksprite, 1, 2, is3x3board, is7x7board, false);
						currentchips.push_back(chip1);

						Chip* chip2 = new Chip(whitesprite, 3, 2, is3x3board, is7x7board, true);
						currentchips.push_back(chip2);

						for (unsigned int i = 1; i <= 3; i++) { // Bottom row of all white chips
							Chip* chip = new Chip(whitesprite, i, 3, is3x3board, is7x7board, true);
							currentchips.push_back(chip);
						}
						
						if (LOG_MASTER && LOG_CHIPSSIZE) {
							cout << "LOG currentchips Size:\t" << currentchips.size() << endl;
						}
					}
					else if (!is3x3board && !is7x7board) { // This is a 5x5 board
						// Clear the currentchips vector just in case there was something inside
						currentchips.clear();

						//=== NORMAL 5X5 GAME BOARD
						for (unsigned int i = 1; i <= 5; i++) { // Top 2 rows of all black chips
							for (unsigned int j = 1; j <= 2; j++) {
								Chip* chip = new Chip(blacksprite, i, j, is3x3board, is7x7board, false);
								currentchips.push_back(chip);
							}
						}

						Chip* chip1 = new Chip(blacksprite, 1, 3, is3x3board, is7x7board, false);
						currentchips.push_back(chip1);
						Chip* chip2 = new Chip(blacksprite, 4, 3, is3x3board, is7x7board, false);
						currentchips.push_back(chip2);

						Chip* chip3 = new Chip(whitesprite, 2, 3, is3x3board, is7x7board, true);
						currentchips.push_back(chip3);
						Chip* chip4 = new Chip(whitesprite, 5, 3, is3x3board, is7x7board, true);
						currentchips.push_back(chip4);
						
						for (unsigned int i = 1; i <= 5; i++) { // Bottom 2 rows of all white chips
							for (unsigned int j = 4; j <= 5; j++) {
								Chip* chip = new Chip(whitesprite, i, j, is3x3board, is7x7board, true);
								currentchips.push_back(chip);
							}
						}

						if (LOG_MASTER && LOG_CHIPSSIZE) {
							cout << "LOG currentchips Size:\t" << currentchips.size() << endl;
						}
					}
					else { // This is the 7x7 board
						// Clear the currentchips vector just in case there was something inside
						currentchips.clear();

						float chipScale = 0.45f;

						//=== Normal 7x7 Game Board
						for (unsigned int i = 1; i <= 7; i++) { // Top 3 rows of all black chips
							for (unsigned int j = 1; j <= 3; j++) {
								Chip* chip = new Chip(blacksprite, i, j, is3x3board, is7x7board, false);
								chip->scale = chipScale;
								currentchips.push_back(chip);
							}
						}

						Chip* chip1 = new Chip(blacksprite, 1, 4, is3x3board, is7x7board, false);
						chip1->scale = chipScale;
						currentchips.push_back(chip1);
						Chip* chip2 = new Chip(blacksprite, 3, 4, is3x3board, is7x7board, false);
						chip2->scale = chipScale;
						currentchips.push_back(chip2);
						Chip* chip3 = new Chip(blacksprite, 6, 4, is3x3board, is7x7board, false);
						chip3->scale = chipScale;
						currentchips.push_back(chip3);

						Chip* chip4 = new Chip(whitesprite, 2, 4, is3x3board, is7x7board, true);
						chip4->scale = chipScale;
						currentchips.push_back(chip4);
						Chip* chip5 = new Chip(whitesprite, 5, 4, is3x3board, is7x7board, true);
						chip5->scale = chipScale;
						currentchips.push_back(chip5);
						Chip* chip6 = new Chip(whitesprite, 7, 4, is3x3board, is7x7board, true);
						chip6->scale = chipScale;
						currentchips.push_back(chip6);

						for (unsigned int i = 1; i <= 7; i++) { // Bottom 3 rows of all white chips
							for (unsigned int j = 5; j <= 7; j++) {
								Chip* chip = new Chip(whitesprite, i, j, is3x3board, is7x7board, true);
								chip->scale = chipScale;
								currentchips.push_back(chip);
							}
						}

						if (LOG_MASTER && LOG_CHIPSSIZE) {
							cout << "LOG currentchips Size:\t" << currentchips.size() << endl;
						}
					}

					//	Change the game state -- white player always goes first
					gamestate = STATE_WHITEPLAYER;

					//	Console output to let the player know that it's White's turn -- white always goes first
					cout << "\n=================== White Player's Turn ===================" << endl;
				}
			} // above is gamestate == STATE_MAINMENU

			else if (gamestate == STATE_WHITEPLAYER || gamestate == STATE_BLACKPLAYER) {
				//	If it's the player's turn, he/she should be allowed to make the move
				bool isplayerTurn = false;
				if (playerisWhite && gamestate == STATE_WHITEPLAYER) {
					isplayerTurn = true;
				}
				else if (!playerisWhite && gamestate == STATE_BLACKPLAYER) {
					isplayerTurn = true;
				}

				//=== TESTING SETTING for HUMAN VS HUMAN
				isplayerTurn = true;
				//===*/

				if (isplayerTurn && !validPiece) {
					//	The player is white and have NOT yet selected a piece to move
					validPieceCol = MouseToBoardCol(mouseX);
					validPieceRow = MouseToBoardRow(mouseY);

					if (LOG_MASTER && LOG_CLICKEDCHIP) {
						cout << "LOG Chip Clicked:\tColumn: " << validPieceCol << "\t\tRow: " << validPieceRow << endl;
					}

					//	If either of the two values (Col || Row) == 0, then the player did not click on a valid piece
					if (validPieceCol == 0 || validPieceRow == 0) { // NOT VALID PIECE
						if (gamestate == STATE_WHITEPLAYER && playerisWhite) {
							cout << "Please click on a valid white chip.\n" << endl;
						}
						else if (gamestate == STATE_BLACKPLAYER && !playerisWhite) {
							cout << "Please click on a valid black chip.\n" << endl;
						}
					}
					else { // Ensure that the piece actually exists && isWhite
						//	Set all chips on board to not have available moves before checking for valid moves
						clearChipsMoves(currentchips);

						for (unsigned int i = 0; i < currentchips.size(); i++) {
							if (currentchips[i]->boardRow == validPieceRow && currentchips[i]->boardCol == validPieceCol) { // Make sure the piece exists
								// Make sure the proper color is pressed
								if ((currentchips[i]->isWhite && gamestate == STATE_WHITEPLAYER) || (!currentchips[i]->isWhite && gamestate == STATE_BLACKPLAYER)) {
									cout << "Your chip selected at: " << validPieceCol << "," << validPieceRow << endl;

									populateChipMoves(currentchips[i], currentchips, is3x3board);	// Find what moves the piece can make

									if (LOG_MASTER && LOG_POPULATECHIPMOVES) {
										cout << "LOG CHIP: withdrawUP\t\t" << currentchips[i]->withdrawUP << endl;
										cout << "LOG CHIP: withdrawDOWN\t\t" << currentchips[i]->withdrawDOWN << endl;
										cout << "LOG CHIP: withdrawLEFT\t\t" << currentchips[i]->withdrawLEFT << endl;
										cout << "LOG CHIP: withdrawRIGHT\t\t" << currentchips[i]->withdrawRIGHT << endl;
										cout << "LOG CHIP: withdrawULEFT\t\t" << currentchips[i]->withdrawULEFT << endl;
										cout << "LOG CHIP: withdrawURIGHT\t" << currentchips[i]->withdrawURIGHT << endl;
										cout << "LOG CHIP: withdrawBLEFT\t\t" << currentchips[i]->withdrawBLEFT << endl;
										cout << "LOG CHIP: withdrawBRIGHT\t" << currentchips[i]->withdrawBRIGHT << endl;
										
										cout << "LOG CHIP: approachUP\t\t" << currentchips[i]->approachUP << endl;
										cout << "LOG CHIP: approachDOWN\t\t" << currentchips[i]->approachDOWN << endl;
										cout << "LOG CHIP: approachLEFT\t\t" << currentchips[i]->approachLEFT << endl;
										cout << "LOG CHIP: approachRIGHT\t\t" << currentchips[i]->approachRIGHT << endl;
										cout << "LOG CHIP: approachULEFT\t\t" << currentchips[i]->approachULEFT << endl;
										cout << "LOG CHIP: approachURIGHT\t" << currentchips[i]->approachURIGHT << endl;
										cout << "LOG CHIP: approachBLEFT\t\t" << currentchips[i]->approachBLEFT << endl;
										cout << "LOG CHIP: approachBRIGHT\t" << currentchips[i]->approachBRIGHT << endl;
									}

									//	The piece is valid if there's at least one method of capture
									if (currentchips[i]->withdrawUP || currentchips[i]->withdrawDOWN || currentchips[i]->withdrawLEFT || currentchips[i]->withdrawRIGHT ||
										currentchips[i]->withdrawULEFT || currentchips[i]->withdrawURIGHT || currentchips[i]->withdrawBLEFT || currentchips[i]->withdrawBRIGHT ||
										currentchips[i]->approachUP || currentchips[i]->approachDOWN || currentchips[i]->approachLEFT || currentchips[i]->approachRIGHT ||
										currentchips[i]->approachULEFT || currentchips[i]->approachURIGHT || currentchips[i]->approachBLEFT || currentchips[i]->approachBRIGHT) {
										validPiece = true;
										cout << "\nThe selected piece is valid for a capturing move.\nPlease select the enemy piece to capture." << endl;

										//	Give sword animations on the pieces that can be attacked
										populatePossibleAttacks(currentchips[i]);
									}
									else {	//	If the piece is NOT valid (cannot capture in any way), check if paika should be allowed
										if (paikaCheck()) {
											// Paika should be enabled
											validPiece = true;
											paikaEnabled = true;
											cout << "A paika move with the selected piece is allowed.\nPlease select the destination.\n" << endl;
										}
										else { // Another piece should be selected
											cout << "This piece cannot capture or paika in this turn.\nPlease select another valid piece." << endl;
										}
									}
								}
							}
						}


					}
				}
				else if (isplayerTurn && validPiece) {
					// The player is white and has selected a valid piece to use -- now the player must choose an enemy piece to capture || paika desination
					targetCol = MouseToBoardCol(mouseX);
					targetRow = MouseToBoardRow(mouseY);

					if (LOG_MASTER && LOG_CLICKEDCHIP) {
						cout << "LOG Mouse Click at:\tColumn: " << targetCol << "\t\tRow: " << targetRow << endl;
					}

					// Verify that neither of the two values are 0 -- that would be an invalid location / piece
					if (targetCol == 0 || targetRow == 0) {
						cout << "Please click on a valid enemy chip or destination." << endl;
					}
					else { //	A piece / destination has been selected
						bool targetIsWhite = false;
						for (unsigned int y = 0; y < currentchips.size(); y++) {
							if (currentchips[y]->boardCol == targetCol && currentchips[y]->boardRow == targetRow) {
								targetIsWhite = (currentchips[y]->isWhite);
							}
						}

						if (validMoveCheck()) { // The chosen position / piece is VALID, process the turn
							if (attacktype != ATK_NULL || attacktype != ATK_PAIKA) {
								// The move is a capturing move!
								if (!paikaEnabled) {
									processCapture();
								}
								endTurn();
							}
							else if (attacktype == ATK_PAIKA) {
								// This should only be allowed if paikaEnabled is true
								endTurn();
							}
						}
						else {	// The chosen position / piece is NOT valid, ask for another piece / position
							cout << "The chosen piece or location is NOT a valid move. Please select another." << endl;						
						}
					}
				}

			} // above is gamestate == STATE_WHITEPLAYER || gamestate == STATE_BLACKPLAYER

			else if (gamestate == STATE_GAMEOVER) {
				// On the game over gamestate we want to display the winner situation (who won or was it a draw?)
				// Offer button to go back to main menu
				if (mouseX >= 0.360 && mouseX <= 1.835 && mouseY >= -1.793 && mouseY <= -1.420) {
					// Button to return to main menu has been pressed
					gamestate = STATE_MAINMENU;
				}

			} // above is gamestate == STATE_GAMEOVER

			else if (gamestate == STATE_GAMERULES1) {
				// There is just one button on this screen and it's to go to the next screen
				if (mouseX >= 1.290 && mouseX <= 1.720 && mouseY >= -1.673 && mouseY <= -1.293) {
					Mix_PlayChannel(-1, button_sound, 0);
					gamestate = STATE_GAMERULES2;
				}
			}
//==================================================================
			else if (gamestate == STATE_GAMERULES2) {
				// There are two buttons on this screen : Left / Right

				// The Left button leads back to STATE_GAMERULES1
				if (mouseX >= -1.710 && mouseX <= -1.300 && mouseY >= -1.693 && mouseY <= -1.307) {
					Mix_PlayChannel(-1, button_sound, 0);
					gamestate = STATE_GAMERULES1;
				}

				// The Right button leads back to STATE_MAINMENU
				if (mouseX >= 1.290 && mouseX <= 1.720 && mouseY >= -1.673 && mouseY <= -1.293) {
					Mix_PlayChannel(-1, button_sound, 0);
					gamestate = STATE_MAINMENU;
				}
			}

		} // above is event.type == SDL_MOUSEBUTTONDOWN

		else if (event.type == SDL_MOUSEBUTTONUP) { // This is just used for button animation purposes on buttons 
			
		}
	}

	// Keyboard Input (UP / DOWN / LEFT / RIGHT controls the music)
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) {
		
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
	
	}
	if (keys[SDL_SCANCODE_UP]) {
		
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		
	}
	
	Update(elapsed);	
	Render();

	return done;
}

/*	This is used for helper animations so players have an easier time seeing moves
*/
void Fanorona::populatePossibleAttacks(Chip* chip) {
	possibleATTACKS.clear();

	int enemyCol;
	int enemyRow;

	float PLIMIT = 270.0f;
	float NLIMIT = 180.0f;
	float new_scale = 1.5;
	SheetSprite attackSprite = SheetSprite(ui_texture, 338.0f / 512.0f, 294.0f / 512.0f, 34.0f / 512.0f, 37.0f / 512.0f);

	//Chip* chip = new Chip(blacksprite, i, 1, is3x3board, is7x7board, false);
	
	//	Look at what moves are available and place flags at those locations
	if (chip->withdrawUP) {
		// What is the enemy location?
		enemyCol = chip->boardCol;
		enemyRow = chip->boardRow + 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawDOWN) {
		enemyCol = chip->boardCol;
		enemyRow = chip->boardRow - 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawLEFT) {
		enemyCol = chip->boardCol + 1;
		enemyRow = chip->boardRow;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawRIGHT) {
		enemyCol = chip->boardCol - 1;
		enemyRow = chip->boardRow;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawULEFT) {
		enemyCol = chip->boardCol + 1;
		enemyRow = chip->boardRow + 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawURIGHT) {
		enemyCol = chip->boardCol - 1;
		enemyRow = chip->boardRow + 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);

	}
	if (chip->withdrawBLEFT) {
		enemyCol = chip->boardCol + 1;
		enemyRow = chip->boardRow - 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->withdrawBRIGHT) {
		enemyCol = chip->boardCol - 1;
		enemyRow = chip->boardRow - 1;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}

	if (chip->approachUP) {
		enemyCol = chip->boardCol;
		enemyRow = chip->boardRow - 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachDOWN) {
		enemyCol = chip->boardCol;
		enemyRow = chip->boardRow + 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachLEFT) {
		enemyCol = chip->boardCol - 2;
		enemyRow = chip->boardRow;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachRIGHT) {
		enemyCol = chip->boardCol + 2;
		enemyRow = chip->boardRow;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachULEFT) {
		enemyCol = chip->boardCol - 2;
		enemyRow = chip->boardRow - 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachURIGHT) {
		enemyCol = chip->boardCol + 2;
		enemyRow = chip->boardRow - 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachBLEFT) {
		enemyCol = chip->boardCol - 2;
		enemyRow = chip->boardRow + 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
	if (chip->approachBRIGHT) {
		enemyCol = chip->boardCol + 2;
		enemyRow = chip->boardRow + 2;
		Chip* chip = new Chip(attackSprite, enemyCol, enemyRow, is3x3board, is7x7board, false);
		chip->rotationPLIMIT = PLIMIT;
		chip->rotationNLIMIT = NLIMIT;
		chip->scale = new_scale;
		possibleATTACKS.push_back(chip);
	}
}

/*	This function will end the turn (by changing the gamestate) 
	It can also be modified to allow for consecutive captures
*/
void Fanorona::endTurn(){
	paikaEnabled = false;
	validPiece = false;
	validPieceCol = 0;
	validPieceRow = 0;
	targetCol = 0;
	targetRow = 0;
	possibleATTACKS.clear();

	attacktype = ATK_NULL;

	Mix_PlayChannel(-1, chipMove_sound, 0);

	// Check if the game is over
	if (gamestate == STATE_WHITEPLAYER) {
		turnCounter++;

		// Count the number of black pieces left -- if 0 then White has won
		int numBlack = 0;
		for (unsigned int i = 0; i < currentchips.size(); i++) {
			if (!currentchips[i]->isWhite) {
				numBlack += 1;
			}
		}

		//=== CONSIDER THE SITUATION FOR A DRAW

		if (numBlack > 0) {
			gamestate = STATE_BLACKPLAYER;
			cout << "\n=================== Black Player's Turn ===================" << endl;
		}
		else { // White has won!
			gameover = GAMEOVER_WHITEWINS;
			gamestate = STATE_GAMEOVER;
		}
		
	}
	else if (gamestate == STATE_BLACKPLAYER) {
		turnCounter++;

		// Count the number of white pieces left -- if 0 then Black has won
		int numWhite = 0;
		for (unsigned int i = 0; i < currentchips.size(); i++) {
			if (currentchips[i]->isWhite) {
				numWhite += 1;
			}
		}

		//=== CONSIDER THE SITUATION FOR A DRAW

		if (numWhite > 0) {
			gamestate = STATE_WHITEPLAYER;
			cout << "\n=================== White Player's Turn ===================" << endl;
		}
		else { // Black has won!
			gameover = GAMEOVER_BLACKWINS;
			gamestate = STATE_GAMEOVER;
		}	
	}
}

/*	
*/
void Fanorona::processCapture(){
	// First we note the color of the attacking team
	bool attackerIsWhite = false;

	// We can look at the turn to see who is the attacker
	if (gamestate == STATE_WHITEPLAYER) {
		attackerIsWhite = true;
	}
	else if (gamestate == STATE_BLACKPLAYER) {
		attackerIsWhite = false;
	}

	bool captureComplete = false;
	while (!captureComplete) {	// Keep capturing valid enemy pieces
		for (unsigned int j = 0; j < currentchips.size(); j++) {
			if (currentchips[j]->boardCol == targetCol && currentchips[j]->boardRow == targetRow) {
				// We found the target chip, now delete it
				delete currentchips[j];
				currentchips.erase(currentchips.begin() + j);

				cout << "Enemy chip at " << targetCol << ", " << targetRow << " captured." << endl;
				break;
			}
		}

		// Assume that the capture is complete unless proven otherwise
		captureComplete = true;

		// Update the next target's column and row
		if (attacktype == ATK_WITHDRAWDOWN || attacktype == ATK_APPROACHUP) {
			// Both withdraw down and approach up will capture pieces in the UP direction
			if (targetRow > 1) {
				targetRow -= 1;
			}
		}
		else if (attacktype == ATK_WITHDRAWUP || attacktype == ATK_APPROACHDOWN) {
			// Both withdraw up and approach down will capture pieces in the DOWNW direction
			if (is3x3board) { // This is the 3x3 board
				if (targetRow < 3) {
					targetRow += 1;
				}
			}
			else if (!is3x3board && !is7x7board) { // This is the 5x5 board
				if (targetRow < 5) {
					targetRow += 1;
				}
			}
			else { // This is the 7x7 board
				if (targetRow < 7) {
					targetRow += 1;
				}
			}
		}
		else if (attacktype == ATK_WITHDRAWRIGHT || attacktype == ATK_APPROACHLEFT) {
			// Both withdraw right and approach left will capture pieces in the LEFT direction
			if (targetCol > 1) {
				targetCol -= 1;
			}
		}
		else if (attacktype == ATK_WITHDRAWLEFT || attacktype == ATK_APPROACHRIGHT) {
			// Both withdraw left and approach right will capture pieces in the RIGHT direction
			if (is3x3board) {
				if (targetCol < 3) {
					targetCol += 1;
				}
			}
			else if (!is3x3board && !is7x7board) { // 5x5 Board
				if (targetCol < 5) {
					targetCol += 1;
				}
			}
			else { // 7x7 Board
				if (targetCol < 7) {
					targetCol += 1;
				}
			}
		}
		else if (attacktype == ATK_WITHDRAWULEFT || attacktype == ATK_APPROACHBRIGHT) {
			// Both withdraw upper left and approach bottom right will capture pieces in a down & right diagonal direction
			if (is3x3board) {
				if (targetCol < 3 && targetRow < 3) {
					targetCol += 1;
					targetRow += 1;
				}
			}
			else if (!is3x3board && !is7x7board) { // This is the 5x5 board
				if (targetCol < 5 && targetRow < 5) {
					targetCol += 1;
					targetRow += 1;
				}
			}
			else { // 7x7 board
				if (targetCol < 7 && targetRow < 7) {
					targetCol += 1;
					targetRow += 1;
				}
			}
		}
		else if (attacktype == ATK_WITHDRAWURIGHT || attacktype == ATK_APPROACHBLEFT) {
			// Both withdraw upper right and approach bottom left will capture pieces in a down & left diagonal direction
			if (is3x3board) {
				if (targetCol > 1 && targetRow < 3) {
					targetCol -= 1;
					targetRow += 1;
				}
			}
			else if (!is3x3board && !is7x7board) { // 5x5 Board
				if (targetCol > 1 && targetRow < 5) {
					targetCol -= 1;
					targetRow += 1;
				}
			}
			else { // 7x7 Board
				if (targetCol > 1 && targetRow < 7) {
					targetCol -= 1;
					targetRow += 1;
				}
			}
		}
		else if (attacktype == ATK_WITHDRAWBLEFT || attacktype == ATK_APPROACHURIGHT) {
			// Both withdraw bottom left and approach upper right will capture pieces in an up & right direction
			if (is3x3board) {
				if (targetCol < 3 && targetRow > 1) {
					targetCol += 1;
					targetRow -= 1;
				}
			}
			else if (!is3x3board && !is7x7board) { // 5x5 Board
				if (targetCol < 5 && targetRow > 1) {
					targetCol += 1;
					targetRow -= 1;
				}
			}
			else { // 7x7 Board
				if (targetCol < 7 && targetRow > 1) {
					targetCol += 1;
					targetRow -= 1;
				}
			}
		}
		else if (attacktype == ATK_WITHDRAWBRIGHT || attacktype == ATK_APPROACHULEFT) {
			// Both withdraw bottom right and approach upper left will capture pieces in an up & left direction
			if (targetCol > 1 && targetRow > 1) {
				targetCol -= 1;
				targetRow -= 1;
			}
		}

		// Check if the updated location is a target
		for (unsigned int k = 0; k < currentchips.size(); k++) {
			if (currentchips[k]->boardRow == targetRow && currentchips[k]->boardCol == targetCol && (currentchips[k]->isWhite != attackerIsWhite)) {
				captureComplete = false;
			}
		}
	}
}

/*	This function will return a bool that will determine whether the piece clicked on by the player is a valid move (capture || paika)
	If the move is valid (true), then the move is implemented, otherwise the player will be asked to input again.
*/
bool Fanorona::validMoveCheck(){
	bool returnvalue = false;
	attacktype = ATK_NULL;

	// If this function is called, we already know that the attacking piece is valid, but we need to verify the target / destination
	for (unsigned int j = 0; j < currentchips.size(); j++) {
		if (currentchips[j]->boardCol == validPieceCol && currentchips[j]->boardRow == validPieceRow) { // We found the attacking chip
			bool attackerisWhite = currentchips[j]->isWhite;
			for (unsigned int i = 0; i < currentchips.size(); i++) {
				if (currentchips[i]->boardCol == targetCol && currentchips[i]->boardRow == targetRow && currentchips[i]->isWhite != attackerisWhite) { // We found the target chip

					int chipRow = validPieceRow;
					int chipCol = validPieceCol;
					
					// Withdraw Moves
					if (currentchips[j]->withdrawUP) {
						if (targetRow == (chipRow + 1) && targetCol == chipCol) {
							attacktype = ATK_WITHDRAWUP;
							cout << "\nPlayer is capturing via Withdraw Up." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->withdrawDOWN) {
						if (targetRow == (chipRow - 1) && targetCol == chipCol) {
							attacktype = ATK_WITHDRAWDOWN;
							cout << "\nPlayer is capturing via Withdraw Down." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardRow += 1;
						}
					}
					if (currentchips[j]->withdrawLEFT) {
						if (targetRow == chipRow && targetCol == (chipCol + 1)) {
							attacktype = ATK_WITHDRAWLEFT;
							cout << "\nPlayer is capturing via Withdraw Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
						}
					}
					if (currentchips[j]->withdrawRIGHT) {
						if (targetRow == chipRow && targetCol == (chipCol - 1)) {
							attacktype = ATK_WITHDRAWRIGHT;
							cout << "\nPlayer is capturing via Withdraw Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
						}
					}
					if (currentchips[j]->withdrawULEFT) {
						if (targetRow == (chipRow + 1) && targetCol == (chipCol + 1)) {
							attacktype = ATK_WITHDRAWULEFT;
							cout << "\nPlayer is capturing via Withdraw Upper-Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->withdrawURIGHT) {
						if (targetRow == (chipRow + 1) && targetCol == (chipCol - 1)) {
							attacktype = ATK_WITHDRAWURIGHT;
							cout << "\nPlayer is capturing via Withdraw Upper-Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->withdrawBLEFT) {
						if (targetRow == (chipRow - 1) && targetCol == (chipCol + 1)) {
							attacktype = ATK_WITHDRAWBLEFT;
							cout << "\nPlayer is capturing via Withdraw Bottom-Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
							currentchips[j]->boardRow += 1;
						}
					}
					if (currentchips[j]->withdrawBRIGHT) {
						if (targetRow == (chipRow - 1) && targetCol == (chipCol - 1)) {
							attacktype = ATK_WITHDRAWBRIGHT;
							cout << "\nPlayer is capturing via Withdraw Bottom-Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
							currentchips[j]->boardRow += 1;
						}
					}
				
					// Approach Moves
					if (currentchips[j]->approachUP) {
						if (targetRow == (chipRow - 2) && targetCol == chipCol) {
							attacktype = ATK_APPROACHUP;
							cout << "\nPlayer is capturing via Approach Up." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->approachDOWN) {
						if (targetRow == (chipRow + 2) && targetCol == chipCol) {
							attacktype = ATK_APPROACHDOWN;
							cout << "\nPlayer is capturing via Approach Down." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardRow += 1;
						}
					}
					if (currentchips[j]->approachLEFT) {
						if (targetRow == chipRow && targetCol == (chipCol - 2)) {
							attacktype = ATK_APPROACHLEFT;
							cout << "\nPlayer is capturing via Approach Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
						}
					}
					if (currentchips[j]->approachRIGHT) {
						if (targetRow == chipRow && targetCol == (chipCol + 2)) {
							attacktype = ATK_APPROACHRIGHT;
							cout << "\nPlayer is capturing via Approach Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
						}
					}
					if (currentchips[j]->approachULEFT) {
						if (targetRow == (chipRow - 2) && targetCol == (chipCol - 2)) {
							attacktype = ATK_APPROACHULEFT;
							cout << "\nPlayer is capturing via Approach Upper-Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->approachURIGHT) {
						if (targetRow == (chipRow - 2) && targetCol == (chipCol + 2)) {
							attacktype = ATK_APPROACHURIGHT;
							cout << "\nPlayer is capturing via Approach Upper-Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
							currentchips[j]->boardRow -= 1;
						}
					}
					if (currentchips[j]->approachBLEFT) {
						if (targetRow == (chipRow + 2) && targetCol == (chipCol - 2)) {
							attacktype = ATK_APPROACHBLEFT;
							cout << "\nPlayer is capturing via Approach Bottom-Left." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol -= 1;
							currentchips[j]->boardRow += 1;
						}
					}
					if (currentchips[j]->approachBRIGHT) {
						if (targetRow == (chipRow + 2) && targetCol == (chipCol + 2)) {
							attacktype = ATK_APPROACHBRIGHT;
							cout << "\nPlayer is capturing via Approach Bottom-Right." << endl;
							returnvalue = true;

							// Move the position of the attacker chip
							currentchips[j]->boardCol += 1;
							currentchips[j]->boardRow += 1;
						}
					}
				}
			}
		}
	}

	//	Consider Paika Moves
	if (paikaEnabled) {
		//	We need the destination to be within one move from the current position
		if ((abs(targetCol - validPieceCol) == 0 && abs(targetRow - validPieceRow) == 1) || (abs(targetCol - validPieceCol) == 1 && abs(targetRow - validPieceRow) == 0)) {
			//	The move is either horizontal or veritical into an empty position -- it's valid
			attacktype = ATK_PAIKA;
			returnvalue = true;
		}
		else if (abs(targetCol - validPieceCol) == 1 && abs(targetRow - validPieceRow) == 1) {
			//	The move is diagonal into an empty position -- check if it's valid
			if ((validPieceCol % 2) == (validPieceRow % 2)) {
				// The chosen chip can move diagonally
				attacktype = ATK_PAIKA;
				returnvalue = true;
			}
		}

		//	We need the target / destination to be open / empty
		for (unsigned int k = 0; k < currentchips.size(); k++) {
			if (currentchips[k]->boardCol == targetCol && currentchips[k]->boardRow == targetRow) {
				//	There's a piece at the target destination -- NOT valid for a paika move
				attacktype = ATK_NULL;
				returnvalue = false;
			}
		}
		
		// Make the Paika Move
		if (attacktype == ATK_PAIKA) {
			for (unsigned int i = 0; i < currentchips.size(); i++) { // Find the paika mover
				if (currentchips[i]->boardCol == validPieceCol && currentchips[i]->boardRow == validPieceRow) {
					cout << "Player Paika moved Chip at (" << currentchips[i]->boardCol << ", " << currentchips[i]->boardRow << ") to (" << targetCol << ", " << targetRow << ")" << endl;

					// Set the position of the chip
					currentchips[i]->boardCol = targetCol;
					currentchips[i]->boardRow = targetRow;
				}
			}
		}
	}

	return returnvalue;
}

/*	Used to check whether the current team has any pieces that can make a capturing move
	If no capture moves can be made, return true -- false otherwise
*/
bool Fanorona::paikaCheck() {
	clearChipsMoves(currentchips);

	for (unsigned int i = 0; i < currentchips.size(); i++) {
		if (gamestate == STATE_WHITEPLAYER) { // It's white's turn, check white pieces
			if (currentchips[i]->isWhite) { // This is a white piece
				// Check moves that the piece can do
				populateChipMoves(currentchips[i], currentchips, is3x3board);

				// If there's a piece that can capture in any way, paika should NOT be enabled
				if (currentchips[i]->withdrawUP || currentchips[i]->withdrawDOWN || currentchips[i]->withdrawLEFT || currentchips[i]->withdrawRIGHT ||
					currentchips[i]->withdrawULEFT || currentchips[i]->withdrawURIGHT || currentchips[i]->withdrawBLEFT || currentchips[i]->withdrawBRIGHT ||
					currentchips[i]->approachUP || currentchips[i]->approachDOWN || currentchips[i]->approachLEFT || currentchips[i]->approachRIGHT ||
					currentchips[i]->approachULEFT || currentchips[i]->approachURIGHT || currentchips[i]->approachBLEFT || currentchips[i]->approachBRIGHT) {
					return false;
				}
			}
		}
		else if (gamestate == STATE_BLACKPLAYER) { // It's black's turn, check black pieces
			if (!(currentchips[i]->isWhite)) { // This is a black piece
				populateChipMoves(currentchips[i], currentchips, is3x3board);

				if (currentchips[i]->withdrawUP || currentchips[i]->withdrawDOWN || currentchips[i]->withdrawLEFT || currentchips[i]->withdrawRIGHT ||
					currentchips[i]->withdrawULEFT || currentchips[i]->withdrawURIGHT || currentchips[i]->withdrawBLEFT || currentchips[i]->withdrawBRIGHT ||
					currentchips[i]->approachUP || currentchips[i]->approachDOWN || currentchips[i]->approachLEFT || currentchips[i]->approachRIGHT ||
					currentchips[i]->approachULEFT || currentchips[i]->approachURIGHT || currentchips[i]->approachBLEFT || currentchips[i]->approachBRIGHT) {
					return false;
				}
			}
		}
	}

	// Now ensure that the selected chip can actually make a Paika Move
	bool paikaUP = true;
	bool paikaDOWN = true;
	bool paikaLEFT = true;
	bool paikaRIGHT = true;
	bool paikaULEFT = true;
	bool paikaURIGHT = true;
	bool paikaBLEFT = true;
	bool paikaBRIGHT = true;
	int chipCol = validPieceCol;
	int chipRow = validPieceRow;

	for (unsigned int i = 0; i < currentchips.size(); i++) {
		if (currentchips[i]->boardCol == validPieceCol && currentchips[i]->boardRow == validPieceRow) { // Found the selected piece
			// Check if we can paika DIAGONALLY
			if ((validPieceCol % 2) != (validPieceRow % 2)) {
				paikaULEFT = false;
				paikaURIGHT = false;
				paikaBLEFT = false;
				paikaBRIGHT = false;
			}

			// Check for boundary issues
			if (validPieceRow == 1) { // Cannot paika UP if it's already the TOP row
				paikaUP = false;
				paikaULEFT = false;
				paikaURIGHT = false;
			}
			if (validPieceCol == 1) { // Cannot paika to the LEFT if it's left-most Column
				paikaLEFT = false;
				paikaULEFT = false;
				paikaBLEFT = false;
			}
			if (is3x3board) {
				if (validPieceRow == 3) {
					paikaDOWN = false;
					paikaBLEFT = false;
					paikaBRIGHT = false;
				}
				if (validPieceCol == 3) {
					paikaRIGHT = false;
					paikaURIGHT = false;
					paikaBRIGHT = false;
				}
			}
			else { // 5x5 Board
				if (validPieceRow == 5) {
					paikaDOWN = false;
					paikaBLEFT = false;
					paikaBRIGHT = false;
				}
				if (validPieceCol == 5) {
					paikaRIGHT = false;
					paikaURIGHT = false;
					paikaBRIGHT = false;
				}
			}

			//	Check for collision between paika piece and other pieces on board
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == chipCol) { // These are pieces on the same column as the paika chip
					if (currentchips[j]->boardRow == (chipRow + 1)) {
						paikaDOWN = false;
					}
					if (currentchips[j]->boardRow == (chipRow - 1)) {
						paikaUP = false;
					}
				}

				if (currentchips[j]->boardCol == (chipCol - 1)) { // These are pieces to the LEFT of the paika chip
					if (currentchips[j]->boardRow == (chipRow)) {
						paikaLEFT = false;
					}
					if (currentchips[j]->boardRow == (chipRow - 1)) {
						paikaULEFT = false;
					}
					if (currentchips[j]->boardRow == (chipRow + 1)) {
						paikaBLEFT = false;
					}
				}

				if (currentchips[j]->boardCol == (chipCol + 1)) { // These are pieces to the RIGHT of the paika chip
					if (currentchips[j]->boardRow == (chipRow)) {
						paikaRIGHT = false;
					}
					if (currentchips[j]->boardRow == (chipRow + 1)) {
						paikaBRIGHT = false;
					}
					if (currentchips[j]->boardRow == (chipRow - 1)) {
						paikaURIGHT = false;
					}
				}
			}
		}
	}

	// As long as one of the paika moves are possible, then return true (will enable paika)
	if (paikaUP || paikaDOWN || paikaLEFT || paikaRIGHT || paikaULEFT || paikaURIGHT || paikaBLEFT || paikaBRIGHT){
		return true;
	}

	return false;
}

/*	MouseToBoardRow returns the row that the player pressed with mouse
*/
int Fanorona::MouseToBoardRow(float mouseY) {
	int Row = 0;
	if (is3x3board) {
		if (mouseY >= 1.047 && mouseY <= 1.627)			{ Row = 1; }
		else if (mouseY >= -0.280 && mouseY <= 0.313)	{ Row = 2; }
		else if (mouseY >= -1.593 && mouseY <= -1.027)	{ Row = 3; }
	}
	else if (!is3x3board && !is7x7board) { // 5x5 Board
		if (mouseY >= 1.400 && mouseY <= 1.967)			{ Row = 1; }
		else if (mouseY >= 0.560 && mouseY <= 1.133)	{ Row = 2; }
		else if (mouseY >= -0.273 && mouseY <= 0.300)	{ Row = 3; }
		else if (mouseY >= -1.113 && mouseY <= -0.540)	{ Row = 4; }
		else if (mouseY >= -1.947 && mouseY <= -1.380)	{ Row = 5; }
	}
	else {	// 7x7 Board
		if (mouseY >= 1.440 && mouseY <= 1.880)			{ Row = 1; }
		else if (mouseY >= 0.900 && mouseY <= 1.320)	{ Row = 2; }
		else if (mouseY >= 0.347 && mouseY <= 0.773)	{ Row = 3; }
		else if (mouseY >= -0.200 && mouseY <= 0.220)	{ Row = 4; }
		else if (mouseY >= -0.760 && mouseY <= -0.333)	{ Row = 5; }
		else if (mouseY >= -1.307 && mouseY <= -0.880)	{ Row = 6; }
		else if (mouseY >= -1.860 && mouseY <= -1.427)	{ Row = 7; }
	}
	return Row;
}

/*	MouseToBoardCol returns the column that the player pressed with mouse
*/
int Fanorona::MouseToBoardCol(float mouseX) {
	int Col = 0; 
	if (is3x3board) {
		if (mouseX >= -1.610 && mouseX <= -1.030)		{ Col = 1; }
		else if (mouseX >= -0.285 && mouseX <= 0.285)	{ Col = 2; }
		else if (mouseX >= 1.020 && mouseX <= 1.600)	{ Col = 3; }
	}
	else if (!is3x3board && !is7x7board) { // 5x5 Board
		if (mouseX >= -1.970 && mouseX <= -1.385)		{ Col = 1; }
		else if (mouseX >= -1.135 && mouseX <= -0.545)	{ Col = 2; }
		else if (mouseX >= -0.300 && mouseX <= 0.295)	{ Col = 3; }
		else if (mouseX >= 0.535 && mouseX <= 1.125)	{ Col = 4; }
		else if (mouseX >= 1.370 && mouseX <= 1.955)	{ Col = 5; }
	}
	else {	// 7x7 Board
		if (mouseX >= -1.865 && mouseX <= -1.440)		{ Col = 1; }
		else if (mouseX >= -1.320 && mouseX <= -0.890)	{ Col = 2; }
		else if (mouseX >= -0.765 && mouseX <= -0.340)	{ Col = 3; }
		else if (mouseX >= -0.215 && mouseX <= 0.210)	{ Col = 4; }
		else if (mouseX >= 0.330 && mouseX <= 0.760)	{ Col = 5; }
		else if (mouseX >= 0.885 && mouseX <= 1.310)	{ Col = 6; }
		else if (mouseX >= 1.435 && mouseX <= 1.860)	{ Col = 7; }
	}
	return Col;
}

/*	clearChipsGo sets all the boolean withdraw / approach / paika variables of the provided vector of Chips to be false
	This should be used every time before searching for valid moves so we have updated information
*/
void Fanorona::clearChipsMoves(vector<Chip*> currentchips) {
	for (unsigned int i = 0; i < currentchips.size(); i++) {
		//	Clear the withdraw captures
		currentchips[i]->withdrawUP = false;
		currentchips[i]->withdrawDOWN = false;
		currentchips[i]->withdrawLEFT = false;
		currentchips[i]->withdrawRIGHT = false;
		currentchips[i]->withdrawULEFT = false;
		currentchips[i]->withdrawURIGHT = false;
		currentchips[i]->withdrawBLEFT = false;
		currentchips[i]->withdrawBRIGHT = false;

		//	Clear the approach captures
		currentchips[i]->approachUP = false;
		currentchips[i]->approachDOWN = false;
		currentchips[i]->approachLEFT = false;
		currentchips[i]->approachRIGHT = false;
		currentchips[i]->approachULEFT = false;
		currentchips[i]->approachURIGHT = false;
		currentchips[i]->approachBLEFT = false;
		currentchips[i]->approachBRIGHT = false;
	}
}

/*	populateChipMoves will set the available moves in the chip object that is provided by looking at the board and the arrangement of chips on that board
	By setting the available moves (approach, withdraw, paika), we will know all the possible moves that this chip can make
*/
void Fanorona::populateChipMoves(Chip* chip, vector<Chip*> currentchips, bool is3x3board) {
	// Get the chip's board positions (row and column) & get the chip's team
	int chipCol = (chip->boardCol);
	int chipRow = (chip->boardRow);
	bool chipWhite = (chip->isWhite);		// If this is true, then the chip is WHITE team, else it's BLACK team
	
	for (unsigned int i = 0; i < currentchips.size(); i++) {
		//==================================================================================================================
		//--- Consider Withdraw Moves
		//==================================================================================================================

		// Consider withdrawUP which requires an empty tile on the row above the chip and an enemy tile on the row below
		if (currentchips[i]->boardRow == (chipRow + 1) && currentchips[i]->boardCol == chipCol && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found under chip -- now we need to verify that no chips are above chip
			chip->withdrawUP = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardRow == (chipRow - 1) && currentchips[j]->boardCol == chipCol) {
					chip->withdrawUP = false;
				}
			}

			// Sanity Check -- cannot withdrawup if chip is on row 1
			if (chip->withdrawUP)	{ if (chipRow == 1) { chip->withdrawUP = false;	} }
		}

		// Consider WITHDRAWDOWN which requires an enemy tile on the row above and an empty tile on the row below
		if (currentchips[i]->boardRow == (chipRow - 1) && currentchips[i]->boardCol == chipCol && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found above chip -- now verify that no chips are below chip
			chip->withdrawDOWN = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardRow == (chipRow + 1) && currentchips[j]->boardCol == chipCol) {
					chip->withdrawDOWN = false;
				}
			}

			// Sanity Check -- cannot withdrawdown if chip is on bottom of the board
			if (chip->withdrawDOWN) {
				if (is3x3board)							{ if (chipRow == 3)	{ chip->withdrawDOWN = false; }	}
				else if (!is3x3board && !is7x7board)	{ if (chipRow == 5) { chip->withdrawDOWN = false; } }
				else									{ if (chipRow == 7) { chip->withdrawDOWN = false; }	}
			}
		}

		// Consider WITHDRAWLEFT which requires an enemy tile on the column to the right of chip and an empty tile to the left of chip
		if (currentchips[i]->boardCol == (chipCol + 1) && currentchips[i]->boardRow == chipRow && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the right of chip -- now verify that no chips are to the left of chip
			chip->withdrawLEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol - 1) && currentchips[j]->boardRow == chipRow) {
					chip->withdrawLEFT = false;
				}
			}

			// Sanity check -- cannot withdraw left if chip is on the leftmost column of the board
			if (chip->withdrawLEFT)	{ if (chipCol == 1) { chip->withdrawLEFT = false; }	}
		}

		// Consider WITHDRAWRIGHT which requires an enemy tile on the column to the left and an empty tile to the right of chip
		if (currentchips[i]->boardCol == (chipCol - 1) && currentchips[i]->boardRow == chipRow && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the left of chip -- now verify that no chips are to the right of chip
			chip->withdrawRIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol + 1) && currentchips[j]->boardRow == chipRow) {
					chip->withdrawRIGHT = false;
				}
			}

			// Sanity check -- cannot withdraw right if chip is on the rightmost column of the board
			if (chip->withdrawRIGHT) {
				if (is3x3board)							{ if (chipCol == 3) { chip->withdrawRIGHT = false; } }
				else if (!is3x3board && !is7x7board)	{ if (chipCol == 5) { chip->withdrawRIGHT = false; } }
				else									{ if (chipCol == 7) { chip->withdrawRIGHT = false; } }
			}
		}

		// Consider WITHDRAWULEFT requires an enemy to the bottom right of chip and an empty tile on the upper left of chip
		if (currentchips[i]->boardCol == (chipCol + 1) && currentchips[i]->boardRow == (chipRow + 1) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the bottom right of chip -- verify that upper left of chip is open
			chip->withdrawULEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol - 1) && currentchips[j]->boardRow == (chipRow - 1)) {
					chip->withdrawULEFT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->withdrawULEFT) {
				if ( (chipCol % 2) != (chipRow % 2) ) {
					chip->withdrawULEFT = false;
				}
			}

			// Sanity Check -- cannot withdraw ULEFT if chip is on leftmost column of the board or the top-most row
			if (chip->withdrawULEFT)	{ if (chipCol == 1 || chipRow == 1) { chip->withdrawULEFT = false; } }
		}

		// Consider WITHDRAWURIGHT which requires an enemy to the bottom left of chip and an empty tile on the upper right of chip
		if (currentchips[i]->boardRow == (chipRow + 1) && currentchips[i]->boardCol == (chipCol - 1) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the bottom left of chip -- verify that upper right of chip is open
			chip->withdrawURIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol + 1) && currentchips[j]->boardRow == (chipRow - 1)) {
					chip->withdrawURIGHT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->withdrawURIGHT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->withdrawURIGHT = false;
				}
			}

			// Sanity Check -- cannot withdraw URIGHT if chip is on rightmost column of the board or the top-most row
			if (chip->withdrawURIGHT)	{ 
				if (is3x3board)							{ if (chipCol == 3 || chipRow == 1) { chip->withdrawURIGHT = false;	} }
				else if (!is3x3board && !is7x7board)	{ if (chipCol == 5 || chipRow == 1) { chip->withdrawURIGHT = false; } }
				else									{ if (chipCol == 7 || chipRow == 1) { chip->withdrawURIGHT = false; } }
			}
		}

		// Consider WITHDRAWBLEFT which requires an enemy to the upper right of chip and an empty tile on the bottom left of chip
		if (currentchips[i]->boardCol == (chipCol + 1) && currentchips[i]->boardRow == (chipRow - 1) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the upper right of chip -- verify that the bottom left of chip is open
			chip->withdrawBLEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol - 1) && currentchips[j]->boardRow == (chipRow + 1)) {
					chip->withdrawBLEFT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->withdrawBLEFT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->withdrawBLEFT = false;
				}
			}

			// Sanity Check -- cannot withdraw BLEFT if chip is on leftmost column of board or the bottom-most row
			if (chip->withdrawBLEFT) { 
				if (is3x3board)							{ if (chipCol == 1 || chipRow == 3) { chip->withdrawBLEFT = false; } }
				else if (!is3x3board && !is7x7board)	{ if (chipCol == 1 || chipRow == 5) { chip->withdrawBLEFT = false; } }
				else									{ if (chipCol == 1 || chipRow == 7) { chip->withdrawBLEFT = false; } }
			}
		}

		// Consider WITHDRAWBRIGHT which requires an enemy to the upper left of chip and an empty tile on the bottom right of chip
		if (currentchips[i]->boardCol == (chipCol - 1) && currentchips[i]->boardRow == (chipRow - 1) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found to the upper left of chip -- verify that the bottom right of chip is empty
			chip->withdrawBRIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol + 1) && currentchips[j]->boardRow == (chipRow + 1)) {
					chip->withdrawBRIGHT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->withdrawBRIGHT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->withdrawBRIGHT = false;
				}
			}

			// Sanity Check -- cannot withdraw BRIGHT if chip is on rightmost column of board or the bottom-most row
			if (chip->withdrawBRIGHT) {
				if (is3x3board)							{ if (chipCol == 3 || chipRow == 3) { chip->withdrawBRIGHT = false;	} }
				else if (!is3x3board && !is7x7board)	{ if (chipCol == 5 || chipRow == 5) { chip->withdrawBRIGHT = false; } }
				else									{ if (chipCol == 7 || chipRow == 7) { chip->withdrawBRIGHT = false; } }
			}
		}

		//==================================================================================================================
		//--- Consider Approach Moves
		//==================================================================================================================

		// Consider APPROACHUP which requires an enemy 2 rows above chip and an empty tile 1 row above chip (same column)
		if (currentchips[i]->boardCol == chipCol && currentchips[i]->boardRow == (chipRow - 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 rows above chip -- verify that one row above chip is empty
			chip->approachUP = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == chipCol && currentchips[j]->boardRow == (chipRow - 1)) {
					chip->approachUP = false;
				}
			}
			// There is no need for a sanity check because if a black piece was found 2 rows up, there are no boundary issues to consider
		}

		// Consider APPROACHDOWN which requires an enemy 2 rows below chip and an empty tile 1 row below chip (same column)
		if (currentchips[i]->boardCol == chipCol && currentchips[i]->boardRow == (chipRow + 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 rows below chip -- verify that one row below chip is empty
			chip->approachDOWN = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == chipCol && currentchips[j]->boardRow == (chipRow + 1)) {
					chip->approachDOWN = false;
				}
			}
			// There is no need for a sanity check because if a black player was found 2 rows down, there are no boundary issues to consider
		}

		// Consider APPROACHLEFT which requires an enemy 2 columns to the left and an empty tile one column to the left (same row)
		if (currentchips[i]->boardRow == chipRow && currentchips[i]->boardCol == (chipCol - 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns left of chip -- verify that one column left of chip is empty
			chip->approachLEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardRow == chipRow && currentchips[j]->boardCol == (chipCol - 1)) {
					chip->approachLEFT = false;
				}
			}
			// There is no need for a sanity check
		}

		// Consider APPROACHRIGHT which requires an enemy 2 columns right of chip and an empty tile one column right of chip
		if (currentchips[i]->boardRow == chipRow && currentchips[i]->boardCol == (chipCol + 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns right of chip -- verify that one column right of chip is empty
			chip->approachRIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardRow == chipRow && currentchips[j]->boardCol == (chipCol + 1)) {
					chip->approachRIGHT = false;
				}
			}
			// There is no need for a sanity check
		}

		// Consider APPROACH ULEFT which requires an enemy 2 columns left and 2 rows up from chip and an empty tile one row up and one column left of chip
		if (currentchips[i]->boardRow == (chipRow - 2) && currentchips[i]->boardCol == (chipCol - 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns left and 2 rows up from chip
			chip->approachULEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++){
				if (currentchips[j]->boardCol == (chipCol - 1) && currentchips[j]->boardRow == (chipRow - 1)) {
					chip->approachULEFT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->approachULEFT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->approachULEFT = false;
				}
			}

			// There is no need for a sanity check
		}

		// Consider APPROACH URIGHT which requires an enemy 2 columns right and 2 rows up from chip and an empty tile 1 column right and 1 row up from chip
		if (currentchips[i]->boardRow == (chipRow - 2) && currentchips[i]->boardCol == (chipCol + 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns right and 2 rows up from chip
			chip->approachURIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++){
				if (currentchips[j]->boardCol == (chipCol + 1) && currentchips[j]->boardRow == (chipRow - 1)) {
					chip->approachURIGHT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->approachURIGHT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->approachURIGHT = false;
				}
			}

			// There is no need for a sanity check
		}

		// Consider APPROACH BLEFT which requires an enemy 2 columns left and 2 rows down from chip and an empty tile 1 column left and 1 row down from chip
		if (currentchips[i]->boardCol == (chipCol - 2) && currentchips[i]->boardRow == (chipRow + 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns left and 2 rows down from chip
			chip->approachBLEFT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++){
				if (currentchips[j]->boardCol == (chipCol - 1) && currentchips[j]->boardRow == (chipRow + 1)) {
					chip->approachBLEFT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->approachBLEFT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->approachBLEFT = false;
				}
			}

			// There is no need for a sanity check
		}

		// Consider APPROACH BRIGHT which requires an enemy 2 columns right and 2 rows down from chip and an empty tile 1 column right and 1 row down from chip
		if (currentchips[i]->boardCol == (chipCol + 2) && currentchips[i]->boardRow == (chipRow + 2) && currentchips[i]->isWhite != chipWhite) {
			// Enemy piece found 2 columns right and 2 rows down from chip
			chip->approachBRIGHT = true;
			for (unsigned int j = 0; j < currentchips.size(); j++) {
				if (currentchips[j]->boardCol == (chipCol + 1) && currentchips[j]->boardRow == (chipRow + 1)) {
					chip->approachBRIGHT = false;
				}
			}

			// Diagonal Move -- Validity check -- ensure that chip is on a position that can move diagonally
			if (chip->approachBRIGHT) {
				if ((chipCol % 2) != (chipRow % 2)) {
					chip->approachBRIGHT = false;
				}
			}

			// There is no need for a sanity check
		}
	}
}

/*	This function will print strings onto the screen
*/
void Fanorona::Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a) {
	// Have to go character by character for the offsets...
	// The 16 is the width of the sprite sheet in terms of the number of sprites
	// We must draw multiple quads -- more complex than a single square -- dynamic mesh

	glBindTexture(GL_TEXTURE_2D, Texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	// We must build vertex by vertex for each quad ...
	// We must account for the offset between letters by using size*i
	// We're transforming manually. We're drawing all the quads in a single draw column.
	// If we used the modelview matrix, the entire thing is affected. We don't want that...
	float texture_size = 1.0f / 16.0f;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), (0.5f * size),
			((size + spacing) * i) + (-0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (0.5f * size) });
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		uvData.insert(uvData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}