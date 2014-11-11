#include "Vector.h"

class Matrix {
public:
	Matrix();

	void identity();
	Matrix inverse();
	Matrix operator* (const Matrix& m2);
	Vector operator* (const Vector& v2);

	union {
		float m[4][4];
		float ml[16];
	};
};