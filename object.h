#ifndef _OBJECT_H
#define _OBJECT_H

#include "ray.h"
#include "vect.h"
#include "color.h"


class Object{

public:
	Object();
	virtual Color getColor();
	virtual Vect getNormalAt(Vect);
	virtual double findIntersectionRay(Ray);
};
#endif