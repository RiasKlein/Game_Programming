#include <math.h>

class Vector{
public:
	Vector();
	Vector(float x, float y, float z);

	void normalize();

	float x;
	float y;
	float z;
	float length();
};