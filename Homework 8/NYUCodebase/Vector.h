#pragma once
#include <math.h>

class Vector{
public:
	Vector();
	Vector(float x, float y, float z);

	void normalize();

	Vector operator + (const Vector& v2);
	Vector operator * (float scalar);

	float x;
	float y;
	float z;
	float length();
};