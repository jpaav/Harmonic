/*
 * Vector3.cpp
 *
 *  Created on: Nov 4, 2016
 *      Author: Joseph
 */

#include "Vector3.h"
#include "math.h"

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
	length = 0;
}

Vector3::Vector3(double val) {
	x = val;
	y = val;
	z = val;
	length = sqrt((val*val)*3);
}

Vector3::Vector3(double xval, double yval, double zval) {
	x = xval;
	y = yval;
	z = zval;
	length = sqrt((xval*xval)+(yval*yval)+(zval*zval));
}

Vector3::~Vector3() {
	// TODO Auto-generated destructor stub
}

double Vector3::distance(Vector3 v1, Vector3 v2)
{
	double dist= sqrt(
			(v1.x-v2.x) * (v1.x-v2.x) +
			(v1.y-v2.y) * (v1.y-v2.y) +
			(v1.z-v2.z) * (v1.z-v2.z)
	);
	return dist;
}

Vector3 Vector3::direction(Vector3 v1, Vector3 v2)
{
	Vector3 newVec;
	newVec.set(
			v1.x+v2.x,
			v1.y+v2.y,
			v1.z+v2.z
	);
	newVec.length = sqrt((newVec.getx()*newVec.getx())+(newVec.gety()*newVec.gety())+(newVec.getz()*newVec.getz()));
	return Vector3::normalize(newVec);
}

void Vector3::setAll(double val)
{
	x = val;
	y = val;
	z = val;
	length = sqrt((val*val)*3);
}

void Vector3::set(double xval, double yval, double zval)
{
	x = xval;
	y = yval;
	z = zval;
	length = sqrt((xval*xval)+(yval*yval)+(zval*zval));
	if(xval>1 || yval>1 || zval>1){
		x /= length;
		y /= length;
		z /= length;
	}
}

void Vector3::calcLength()
{
	length = sqrt((x*x)+(y*y)+(z*z));
}

Vector3 Vector3::normalize(Vector3 vec)
{
	Vector3 newVec;
	newVec.x = vec.x/vec.length;
	newVec.y = vec.y/vec.length;
	newVec.z = vec.z/vec.length;
	newVec.length = vec.length;
	return newVec;

}

void Vector3::print()
{
	std::cout << "(" << x << ", " << y << ", " << z << ")\n";
}
