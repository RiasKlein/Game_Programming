#pragma once
#include "SheetSprite.h"

class Chip {
public:
	Chip();
	Chip(SheetSprite sprite, int boardCol , int boardRow, bool is3x3board, bool is7x7board, bool isWhite);

	SheetSprite sprite;

	//	Functions
	void Update(float elapsed);
	void Render();

	//	Variables required for SheetSprite::Draw
	float x = 0.0f;
	float y = 0.0f;
	float scale = 0.6f;
	float rotation = 180.0f;
	bool flipped = false;

	//	For animations
	float rotationPLIMIT = 0.0f;		// Used for animations
	float rotationNLIMIT = 0.0f;		// Used for animations
	bool rotateUP = true;
	
	int boardCol;			//	The column that the chip is at (on the board)
	int boardRow;			//	The row that the chip is at (on the board)

	bool is7x7board;
	bool is3x3board;
	bool isWhite;

	//	Withdraw Moves
	bool withdrawUP = false;
	bool withdrawDOWN = false;
	bool withdrawLEFT = false;
	bool withdrawRIGHT = false;
	bool withdrawULEFT = false;
	bool withdrawURIGHT = false;
	bool withdrawBLEFT = false;
	bool withdrawBRIGHT = false;

	//	Approach Moves
	bool approachUP = false;
	bool approachDOWN = false;
	bool approachLEFT = false;
	bool approachRIGHT = false;
	bool approachULEFT = false;
	bool approachURIGHT = false;
	bool approachBLEFT = false;
	bool approachBRIGHT = false;
};