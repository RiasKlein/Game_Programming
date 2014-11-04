#include "Player.h"

Player::Player() {}

void Player::Update(float elapsed) {
	//	Prevent player from leaving the screen

	//	X-Borders [-1.33, 1.33]
	if (x <= -1.28f) {
		x = -1.28f;
	}
	else if (x >= 1.28f) {
		x = 1.28f;
	}

	//	Y-Borders [-1.00, 1.00]
	if (y <= -0.95f) {
		y = -0.95f;
	}
	else if (y >= 0.95f) {
		y = 0.95f;
	}
}

void Player::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(matrix.ml);
	sprite.Draw(scale_x, scale_y);

	glPopMatrix();
}
