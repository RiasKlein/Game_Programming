#include "Vector.h"

Vector::Vector() {}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length() {
	return sqrt(x * x + y * y + z * z);
}

void Vector::normalize() {
	float length = Vector::length();

	x = x / length;
	y = y / length;
	z = z / length;
}

Vector Vector::operator + (const Vector& v2) {
	Vector result;

	result.x = v2.x + x;
	result.y = v2.y + y;
	result.z = v2.z + z;

	return result;
}

Vector Vector::operator * (float scalar) {
	Vector result;

	result.x = scalar * x;
	result.y = scalar * y;
	result.z = scalar * z;

	return result;
}