#include "Bullet.h"

Bullet::Bullet() {
	x = 15.0f;
	y = 15.0f;
	scale_x = 0.75f;
	scale_y = 0.75f;
}

void Bullet::Update(float elapsed) {
	x += velocity_x * sin(rotation)  * elapsed;
	y += velocity_y * -cos(rotation) * elapsed;
}

void Bullet::Render() {
	if (visible) {
		buildMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glMultMatrixf(matrix.ml);
		sprite.Draw(scale_x, scale_y);

		glPopMatrix();
	}
}