#include "sphere.h"
#include <iostream>
using namespace std;

Sphere::Sphere(){

	center = Vect(0,0,0);
	radius = 1;
	color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect p, double r, Color c) 
	: center(p), radius(r), color(c) {}

Vect Sphere::getSphereCenter(){

	return center;
}

double Sphere::getSphereRadius(){

	return radius;
}

Color Sphere::getColor(){

	return color;
}

//normal always points away from the center of the sphere
Vect Sphere::getNormalAt(Vect point){

	Vect normal_vect = point.vectAdd(center.invert()).normalize();
	return normal_vect;
}

//returns distance from origin of ray to intersection point on sphere
double Sphere::findIntersectionRay(Ray ray){

	Vect ray_origin = ray.getRayOrigin();
	double ray_origin_x = ray_origin.getVectX();
	double ray_origin_y = ray_origin.getVectY();
	double ray_origin_z = ray_origin.getVectZ();

	Vect ray_direction = ray.getRayDirection();
	double ray_direction_x = ray_direction.getVectX();
	double ray_direction_y = ray_direction.getVectY();
	double ray_direction_z = ray_direction.getVectZ();

	Vect sphere_center = center;
	double sphere_center_x = sphere_center.getVectX();
	double sphere_center_y = sphere_center.getVectY();
	double sphere_center_z = sphere_center.getVectZ();

	double a = 1; //normalized
	double b = (2*(ray_origin_x - sphere_center_x) * ray_direction_x) + (2*(ray_origin_y - sphere_center_y) * ray_direction_y) + (2*(ray_origin_z - sphere_center_z) * ray_direction_z);
	double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius*radius);

	double discriminant = b*b - 4*a*c;

	//if the ray intersects the sphere
	if(discriminant > 0){

		//use the root closer to the camera which is the intersection of the ray going into the sphere
		double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.00000001;	//the 00001 helps with accuracy errors

		//if the first root is the smallest positive root
		if(root_1 > 0){

			return root_1;
		}
		else{
			//the second root is the smallest positive root

			double root_2 = ((sqrt(discriminant) - b)/2) - 0.00000001;

			return root_2;
		}
	}
	else{
		//there was no intersection with the ray and the sphere
		return -1;
	}
}