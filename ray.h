#ifndef _RAY_H
#define _RAY_H

#include "vect.h"


class Ray{
	Vect origin, direction;

public:
	Ray();
	Ray(Vect, Vect);
	//method functions
	Vect getRayOrigin();
	Vect getRayDirection();
};
#endif