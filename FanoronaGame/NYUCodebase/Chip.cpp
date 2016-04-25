#include "Chip.h"

Chip::Chip() {}
Chip::Chip(SheetSprite sprite, int boardCol, int boardRow, bool is3x3board, bool is7x7board, bool isWhite) : 
		sprite(sprite), boardCol(boardCol), boardRow(boardRow), is3x3board(is3x3board), is7x7board(is7x7board), isWhite(isWhite) {}

void Chip::Update(float elapsed) {
	float speedup = 180.0f;

	if (rotateUP) { // rotateUP cycle
		if (rotation < rotationPLIMIT) {
			rotation += elapsed * speedup;
		}
		else {
			rotateUP = false;
			rotation -= elapsed * speedup;
		}
	}
	else { // rotateDOWN cycle
		if (rotation > rotationNLIMIT) {
			rotation -= elapsed * speedup;
		}
		else {
			rotateUP = true;
			rotation += elapsed * speedup;
		}
	}
}

void Chip::Render(){
	//	Convert the chip's boardX and boardY coordinates into x and y coordinates
	if (is3x3board) { // This is a 3x3 board
		x = (float)(boardCol - 2) * 1.320f;
		y = (float)(2 - boardRow) * 1.320f;
	}
	else if (!is3x3board && !is7x7board) { // This is a 5x5 board -- Assume that the top left corner is 1,1 ; (boardY, boardX) ; (row, column)
		x = (float)(boardCol - 3) * 0.835f;
		y = (float)(3 - boardRow) * 0.835f;
	}
	else { // This is the 7x7 board
		x = (float)(boardCol - 4) * 0.550f;
		y = (float)(4 - boardRow) * 0.550f;
	}

	// Drawing the chip onto the display
	sprite.Draw(x, y, scale, rotation, flipped);
}
