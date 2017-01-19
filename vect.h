#ifndef _VECT_H
#define _VECT_H

#include <math.h>


class Vect{
	double x, y, z;

public:
	Vect();
	Vect(double, double, double);
	//method functions
	double getVectX();
	double getVectY();
	double getVectZ();
	double magnitude();
	Vect normalize();
	Vect invert();
	double dotProduct(Vect v);
	Vect crossProduct(Vect v);
	Vect vectAdd(Vect v);
	Vect scalarMult(double scalar);
};
#endif
