#ifndef _COLOR_H
#define _COLOR_H

class Color{

//special color is reflectivity
double red, green, blue, special;

public:
	Color();
	Color(double r, double g, double b, double s);
	//methods
	double getColorR();
	double getColorG();
	double getColorB();
	double getColorSpecial();
	double setColorR(double r);
	double setColorG(double g);
	double setColorB(double b);
	double setColorSpecial(double s);
	Color colorScalar(double);
	Color colorAdd(Color);
	Color colorMult(Color);
	Color colorAverage(Color);
	Color clip();
};
#endif