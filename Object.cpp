#include "object.h"
#include <iostream>
using namespace std;

Object::Object(){
	
}

Color Object::getColor(){
	return Color(0.0, 0.0, 0.0, 0.0);
}

Vect Object::getNormalAt(Vect pos){

	return Vect(0,0,0);
}

double Object::findIntersectionRay(Ray ray){
	//cout << "In Object" << endl;
	return 0;
}