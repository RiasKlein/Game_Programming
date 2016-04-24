#include "Entity.h"

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;

	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;

	mass = 0.0f;

	isStatic = false;
}

Entity::~Entity() {}

void Entity::Update(float elapsed) {
	// Let's not let the asteroids escape too far

	if (x <= -1.33f) {
		velocity_x = 0.25f;
		//x = -x;
	}
	else if (x >= 1.33f) {
		velocity_x = -0.25f;
		//x = -x;
	}
	
	if (y <= -1.0f) {
		velocity_y = 0.25f;
		//y = -y;
	}
	else if (y >= 1.0f) {
		velocity_y = -0.25f;
		//y = -y;
	}
}

void Entity::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(matrix.ml);
	sprite.Draw(scale_x, scale_y);

	glPopMatrix();
}

void Entity::FixedUpdate() {

}

void Entity::buildMatrix() {
	Matrix scaleMatrix;
	scaleMatrix.identity();
	scaleMatrix.m[0][0] = scale_x;
	scaleMatrix.m[1][1] = scale_y;
	scaleMatrix.m[2][2] = 1;

	Matrix rotateMatrix;
	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(rotation);
	rotateMatrix.m[0][1] = sin(rotation);
	rotateMatrix.m[1][0] = -sin(rotation);
	rotateMatrix.m[1][1] = cos(rotation);

	Matrix translateMatrix;
	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;
	translateMatrix.m[3][2] = 1;

	matrix.identity();
	matrix = translateMatrix * rotateMatrix * scaleMatrix;
}