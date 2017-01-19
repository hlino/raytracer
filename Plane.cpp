#include "plane.h"
#include <iostream>
using namespace std;

Plane::Plane(){

	//default orientation in x/up direction
	normal = Vect(1,0,0);
	distance = 0.0;
	color = Color(0.5,0.5,0.5,0);
}

Plane::Plane(Vect nv, double dv, Color c)
	: normal(nv), distance(dv), color(c) {}

Vect Plane::getPlaneNormal(){

	return normal;
}

double Plane::getPlaneDistance(){

	return distance;
}

Color Plane::getColor(){

	return color;
}

//normal vector on a plane is constant for all positions on plane
Vect Plane::getNormalAt(Vect point){

	return normal;
}

//returns distance from ray origin to the point of intersection
double Plane::findIntersectionRay(Ray ray){

	Vect ray_direction = ray.getRayDirection();
	double a = ray_direction.dotProduct(normal);

	//ray is parallel to plane
	if(a == 0){

		return -1;
	}
	else{

		double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.scalarMult(distance).invert()));
		return -1*b/a;
	}
}