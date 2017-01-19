#ifndef _SPHERE_H
#define _SPHERE_H

#include <math.h>
#include "object.h"
#include "vect.h"
#include "color.h"


class Sphere : public Object {
	Vect center;
	double radius;
	Color color;

public:
	Sphere();
	Sphere(Vect, double, Color);
	Vect getSphereCenter();
	double getSphereRadius();
	virtual Color getColor();
	virtual Vect getNormalAt(Vect);
	virtual double findIntersectionRay(Ray);
};
#endif