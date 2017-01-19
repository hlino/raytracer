#ifndef _CAMERA_H
#define _CAMERA_H

#include "vect.h"

class Camera{

Vect camPos, camDir, camRight, camDown;

public:
	Camera();
	Camera(Vect, Vect, Vect, Vect);
	Vect getCameraPosition();
	Vect getCameraDirection();
	Vect getCameraRight();
	Vect getCameraDown();
};
#endif