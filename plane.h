#ifndef _PLANE_H
#define _PLANE_H

#include <math.h>
#include "object.h"
#include "vect.h"
#include "color.h"


class Plane : public Object {
	Vect normal;
	double distance;
	Color color;

public:
	Plane();
	Plane(Vect, double, Color);
	Vect getPlaneNormal();
	double getPlaneDistance();
	virtual Color getColor();
	virtual Vect getNormalAt(Vect);
	virtual double findIntersectionRay(Ray);
};
#endif