#include "camera.h"

Camera::Camera(){

	camPos = Vect(0,0,0);
	camDir = Vect(0,0,1);
	camRight = Vect(0,0,0);
	camDown = Vect(0,0,0);
}

Camera::Camera(Vect pos, Vect dir, Vect right, Vect down) 
	: camPos(pos), camDir(dir), camRight(right), camDown(down){}

Vect Camera::getCameraPosition(){
	return camPos;
}

Vect Camera::getCameraDirection(){
	return camDir;
}
Vect Camera::getCameraRight(){
	return camRight;
}

Vect Camera::getCameraDown(){
	return camDown;
}