#include "color.h"

Color::Color(){

	red = 0.5;
	green = 0.5;
	blue = 0.5;
	special = 0.5;
}

Color::Color(double r, double g, double b, double s){

	red = r;
	green = g;
	blue = b;
	special = s;
}

double Color::getColorR(){

	return red;
}

double Color::getColorG(){

	return green;
}

double Color::getColorB(){

	return blue;
}

double Color::getColorSpecial(){

	return special;
}

double Color::setColorR(double r){

	red = r;
	return red;
}
double Color::setColorG(double g){

	green = g;
	return green;
}
double Color::setColorB(double b){

	blue = b;
	return blue;
}
double Color::setColorSpecial(double s){

	special = s;
	return special;
}

Color Color::colorScalar(double scalar){
	return Color(red*scalar, green*scalar, blue*scalar, special);
}

Color Color::colorAdd(Color c){
	return Color(red + c.getColorR(), green+c.getColorG(), blue+c.getColorB(), special);
}

Color Color::colorMult(Color c){

	return Color(red*c.getColorR(), green*c.getColorG(), blue*c.getColorB(), special);
}

Color Color::colorAverage(Color c){

	return Color((red + c.getColorR())/2, (green+c.getColorG())/2, (blue+c.getColorB())/2, special);
}

Color Color::clip(){

	double alllight = red +green + blue;
	double exesslight = alllight - 765;
	if(exesslight > 0){
		red = red + exesslight * (red/alllight);
		green = green + exesslight * (green/alllight);
		blue = blue + exesslight * (blue/alllight);
	}
	if(red > 255){
		red = 255;
	}
	if(green > 255){
		green = 255;
	}
	if(blue > 255){
		blue = 255;
	}
	if(red < 0){
		red = 0;
	}
	if(green < 0){
		green = 0;
	}
	if(blue < 0){
		blue = 0;
	}

	return Color(red, green, blue, special);
}