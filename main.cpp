#include <FreeImage.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <cstdlib>

#include "vect.h"
#include "ray.h"
#include "camera.h"
#include "color.h"
#include "light.h"
#include "sphere.h"
#include "object.h"
#include "plane.h"

#define WIDTH  640
#define HEIGHT 480

using namespace std;

BYTE my_image[3 * WIDTH * HEIGHT];

void set_pixel(int x, int y, BYTE R, BYTE G, BYTE B)
{
  my_image[3*y*WIDTH + 3*x + 0] = B;
  my_image[3*y*WIDTH + 3*x + 1] = G;
  my_image[3*y*WIDTH + 3*x + 2] = R;
}

void set_colors() {
  int x, y;

  // Set everything to black
  bzero(my_image, sizeof(my_image));

  // For each pixel in the image
  for (y = 0; y < HEIGHT; y++) {
  	for(x = 0; x < WIDTH; x++){

  		set_pixel(x, y, 0.0, 255.0, 0.0);
  	}
  }
}

void save_image(int w, int h, const char *fname)
{
  FreeImage_Initialise();
  FIBITMAP *img = FreeImage_ConvertFromRawBits(my_image,
                w, h,
                3*w,
                24,
                0, 0, 0,
                false);
  printf("Saving %d x %d image [%s]\n", w, h, fname);
  FreeImage_Save(FIF_PNG, img, fname, 0);
  FreeImage_DeInitialise();
}

//stack to store objects that are in the scene
vector<Object*> scene_objects;
//stack to store the lights in the scene
vector<Light*> scene_lights;
//accuracy value to handle error
double accuracy = 0.0000001;

int closetObjectIndex(vector<double> object_intersections){

	//this part is supposed to be for optimization, but it can probably be removed

	//if there are no intersections
	if(object_intersections.size() == 0){
		return -1;
	}
	else if(object_intersections.size() == 1){

		//if the one intersection distance is positive
		if(object_intersections.at(0) > 0){

			return 0;
		}
		else{
			//if only val in array then the ray missed everything, return -1
			return -1;
		}
	}
	else{

		//finding max value in array
		double max = 0;
		for(float i = 0; i < object_intersections.size(); i++){
			if(object_intersections.at(i) > max){
				max = object_intersections.at(i);
			}
		}

		//if there are valid intersections in the array
		if(max > 0){
			//finding min object index
			double min = max;
			for(float i = 0; i < object_intersections.size(); i++){
				if(object_intersections.at(i) > 0 && object_intersections.at(i) <= min){
					min = i;
				}
			}

			return (int)min;
		}
		else{
			//all intersections were negative, no hits
			return -1;
		}
	}
}

Color getColorAt(int closest_object_index, Vect intersect_position, Vect intersect_ray_direction, double ambient){

	Color closest_object_color = scene_objects.at(closest_object_index)->getColor();
	Vect closest_object_normal = scene_objects.at(closest_object_index)->getNormalAt(intersect_position);

	//make checkered pattern on plane
	if(closest_object_color.getColorSpecial() >= 2 && closest_object_color.getColorSpecial() < 3){

		int square = (int)floor(intersect_position.getVectX()) + (int)floor(intersect_position.getVectZ());

		if((square % 2) == 0){

			closest_object_color.setColorR(0);
			closest_object_color.setColorG(0);
			closest_object_color.setColorB(0);
		}
		else{

			closest_object_color.setColorR(255);
			closest_object_color.setColorG(255);
			closest_object_color.setColorB(255);
		}
	}

	//initial light is scaled by the ambient light value
	Color final_color = closest_object_color.colorScalar(ambient);

	//handels reflection off of objects that have a specular intensity
	if(closest_object_color.getColorSpecial() > 0){

		if(closest_object_color.getColorSpecial() > 1){

			closest_object_color.setColorSpecial(closest_object_color.getColorSpecial() - 1.0);
		}

		double dot1 = closest_object_normal.dotProduct(intersect_ray_direction.invert());
		Vect scalar1 = closest_object_normal.scalarMult(dot1);
		Vect add1 = scalar1.vectAdd(intersect_ray_direction);
		Vect scalar2 = add1.scalarMult(2);
		Vect add2 = intersect_ray_direction.invert().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray(intersect_position, reflection_direction);

		//determine what the reflection ray intersects with first
		vector<double> reflection_intersections;

		for(float reflect_index = 0; reflect_index < scene_objects.size(); reflect_index++){

			reflection_intersections.push_back(scene_objects.at(reflect_index)->findIntersectionRay(reflection_ray));
		}

		int closest_intersection_from_reflection = closetObjectIndex(reflection_intersections);

		//if the reflection ray hit somthing
		if(closest_intersection_from_reflection != -1){
			if(reflection_intersections.at(closest_intersection_from_reflection) > accuracy){

				Vect reflection_intersect_position = intersect_position.vectAdd(reflection_direction.scalarMult(reflection_intersections.at(closest_intersection_from_reflection)));
				Vect reflection_intersect_ray_direction = reflection_direction;

				Color reflection_intersect_color = getColorAt(closest_intersection_from_reflection, reflection_intersect_position, reflection_intersect_ray_direction, ambient);

				final_color = final_color.colorAdd(reflection_intersect_color.colorScalar(closest_object_color.getColorSpecial()));
			}
		}
	}

	//shoots rays from the intersection point to all light sources
	for(float i = 0; i < scene_lights.size(); i++){

		Vect light_direction = scene_lights.at(i)->getLightPosition().vectAdd(intersect_position.invert()).normalize();

		//cosine of the incoming ray and the surface normal
		float lambert = closest_object_normal.dotProduct(light_direction);

		//if the point is not in a shadow
		if(lambert > 0){
			bool shadowed = false;

			Vect distance_to_light = scene_lights.at(i)->getLightPosition().vectAdd(intersect_position.invert()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray(intersect_position, distance_to_light);
			vector<double>secondary_intersections;

			//gets all intersections from the shadow_ray
			for(float object_index = 0; object_index < scene_objects.size() && !shadowed; object_index++){

				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersectionRay(shadow_ray));
			}
			
			//If there is an intersection from the object to the light whose distance is less than distance_to_light then the object is shadowed
			for(float c = 0; c < secondary_intersections.size(); c++){
				if(secondary_intersections.at(c) > accuracy){
					if(secondary_intersections.at(c) <= distance_to_light_magnitude){
						shadowed = true;
					}
					break;
				}
			}

			//if object is not shadowed than the light is applied to it's color
			if(!shadowed){
				final_color = final_color.colorAdd(closest_object_color.colorMult(scene_lights.at(i)->getLightColor()).colorScalar(lambert));

				
				// if special is [0-1]
				if(closest_object_color.getColorSpecial() > 0 &&  closest_object_color.getColorSpecial() <= 1.0) { 
					
					double dot1 = closest_object_normal.dotProduct(intersect_ray_direction.invert());
					Vect scalar1 = closest_object_normal.scalarMult(dot1);
					Vect add1 = scalar1.vectAdd(intersect_ray_direction);
					Vect scalar2 = add1.scalarMult(2);
					Vect add2 = intersect_ray_direction.invert().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);

					if(specular > 0){

						//cout << "HERE" << endl;

						specular = pow(specular, 10);
						final_color = final_color.colorAdd(scene_lights.at(i)->getLightColor().colorScalar(specular*closest_object_color.getColorSpecial()));
					}
					
				}
			}
		}
	}

	return final_color.clip();
}

int main() {

	clock_t t1, t2;
	t1 = clock();

	//defining x y and z direction
	Vect X(1,0,0); //left and right
	Vect Y(0,1,0); //up and down
	Vect Z(0,0,1); //depth into scene
	Vect O(0,-.5,0); //Origin Vector

	//antialiasing depth
	int aadepth = 5;
	double aathreshold = 0.1;
	double aspectratio = (double)WIDTH/(double)HEIGHT;
	//offset of rays from the direction that the camera is pointing
	double xamnt, yamnt;

	//camera set up
	Vect camPos(3, 1.5, -4);
	Vect look_at(0,0,0);
	Vect diff_btw (camPos.getVectX() - look_at.getVectX(), camPos.getVectY() - look_at.getVectY(), camPos.getVectZ() - look_at.getVectZ());
	Vect camDir = diff_btw.invert().normalize();
	Vect camRight = Y.crossProduct(camDir).normalize();
	Vect camDown = camRight.crossProduct(camDir);
	Camera scene_cam (camPos, camDir, camRight, camDown);

	//initializing basic colors
	Color white_light(0.5, 0.5, 0.5, 0);	//ligting colors range from 0-1
	Color pretty_green(127.5, 255.0, 127.5, 0.3);
	Color maroon(127.5, 63.75, 63.75, 2.5);
	Color gray(160.0, 160.0, 160.0, 0.0);
	Color silver(224, 224, 224, 3.0);
	Color black(0,0,0,0);
	Color mirror(255.0, 255.0, 255.0, 1);
	Color matteOrgange(255.0, 128.0, 0.0, 0.0);

	//creating light source
	Vect light_position(-7,10,-10);
	Light scene_point_light(light_position, white_light);
	Light scene_point_light1(Vect(7, 10, -10), white_light);
	//pushing point light to the light stack
	scene_lights.push_back(dynamic_cast<Light*>(&scene_point_light));
	scene_lights.push_back(dynamic_cast<Light*>(&scene_point_light1));
	double ambient_light = 0.02;

	//creating spheres for scene
	Sphere scene_sphere(O, .5, pretty_green);
	Sphere scene_sphere1(Vect(0, .5, 0), .5, matteOrgange);
	//creating plane for scene
	Plane scene_plane(Y, -1, maroon);

	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere1));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));


	// Set everything to black
	bzero(my_image, sizeof(my_image));

	double tempRed, tempGreen, tempBlue;

	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){

			//start with a blank pixel
			double tempRed = 0.0;
			double tempGreen = 0.0;
			double tempBlue  = 0.0;
			int aa_index = 0;

			//for anti aliasing
			for(int aax = 0; aax < aadepth; aax++){
				for(int aay = 0; aay < aadepth; aay++){

					aa_index = aay*aadepth + aax;
					srand(time(0));

					//aadepth of 1 corresponds to no antialiasing
					if(aadepth == 1){

						if(WIDTH > HEIGHT) {
							//the image is wider than it is tall
							xamnt = ((x + 0.5)/WIDTH)*aspectratio - (((WIDTH-HEIGHT)/(double)HEIGHT)/2);
							yamnt = ((HEIGHT - y) + 0.5)/HEIGHT;
						}
						else if(HEIGHT > WIDTH){
							//the image is taller than wide
							xamnt = (x + 0.5)/WIDTH;
							yamnt = (((HEIGHT - y) + 0.5)/HEIGHT)/aspectratio - (((HEIGHT - WIDTH)/(double)WIDTH)/2);
						}
						else{
							//the image is a square
							xamnt = (x + 0.5)/WIDTH;
							yamnt = ((HEIGHT - y) + 0.5)/HEIGHT;
						}
					}
					else{

						if(WIDTH > HEIGHT) {
							//the image is wider than it is tall
							xamnt = ((x + (double)aax/((double)aadepth - 1))/WIDTH)*aspectratio - (((WIDTH-HEIGHT)/(double)HEIGHT)/2);
							yamnt = ((HEIGHT - y) + (double)aax/((double)aadepth - 1))/HEIGHT;
						}
						else if(HEIGHT > WIDTH){
							//the image is taller than wide
							xamnt = (x + (double)aax/((double)aadepth - 1))/WIDTH;
							yamnt = (((HEIGHT - y) + (double)aax/((double)aadepth - 1))/HEIGHT)/aspectratio - (((HEIGHT - WIDTH)/(double)WIDTH)/2);
						}
						else{
							//the image is a square
							xamnt = (x + (double)aax/((double)aadepth - 1))/WIDTH;
							yamnt = ((HEIGHT - y) + (double)aax/((double)aadepth - 1))/HEIGHT;
						}
					}

					//ray origin will be the same origin as the camera
					Vect cam_ray_origin = scene_cam.getCameraPosition();
					//ray direction depends on current pixel and aspect ratio
					Vect cam_ray_direction = camDir.vectAdd(camRight.scalarMult(xamnt - 0.5).vectAdd(camDown.scalarMult(yamnt - 0.5))).normalize();
					//cam_ray is going through specific x y pixel with specified origin and direction
					Ray cam_ray(cam_ray_origin, cam_ray_direction);

					//get intersections with ray and determine if they are with objects in the scene
					vector<double> intersections;

					//for every object in the scene find interesection with current ray and push it to intersections array
					for(float index = 0; index < scene_objects.size(); index++){

						double val = scene_objects.at(index)->findIntersectionRay(cam_ray); 
						//cout << "Value" << val << endl;
						intersections.push_back(val);
					}

					//gets the closed object which the ray intersected, this determines which object was intersected first
					int index_of_closest_object = closetObjectIndex(intersections);
					
					if(index_of_closest_object != -1){
						if(intersections.at(index_of_closest_object) > accuracy){

							Vect intersect_position = cam_ray_origin.vectAdd(cam_ray_direction.scalarMult(intersections.at(index_of_closest_object)));
							Vect intersection_ray_direction = cam_ray_direction;
							Color pixel_color = getColorAt(index_of_closest_object, intersect_position, intersection_ray_direction, ambient_light);
							tempRed = tempRed + pixel_color.getColorR();
							tempGreen = tempGreen + pixel_color.getColorG();
							tempBlue = tempBlue + pixel_color.getColorB();
						}
					}
					else{
							tempRed = tempRed + 0.0;
							tempGreen = tempGreen + 0.0;
							tempBlue = tempBlue + 153.0;
					}
				}
			}

			set_pixel(x, y, tempRed/(aadepth*aadepth), tempGreen/(aadepth*aadepth), tempBlue/(aadepth*aadepth));
		}
	}

  	save_image(WIDTH, HEIGHT, "scene_mirror.png");

  	t2 = clock();
  	float diff = ((float)t2 - (float)t1) / (float)CLOCKS_PER_SEC;
  	cout << "Rendered in " << diff << " seconds." << endl;
  	return 0;
}