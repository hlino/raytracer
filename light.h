#ifndef _LIGHT_H
#define _LIGHT_H

#include "vect.h"
#include "color.h"


class Light{
	Vect position;
	Color color;

public:
	Light();
	Light(Vect, Color);
	Vect getLightPosition();
	Color getLightColor();
};
#endif