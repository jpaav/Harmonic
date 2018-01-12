/*
 * Vector3.h
 *
 *  Created on: Nov 4, 2016
 *      Author: Joseph
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <stdio.h>
#include <iostream>

class Vector3 {
private:
	double x;
	double y;
	double z;
	double length;
public:
	Vector3();
	Vector3(double);
	Vector3(double,double,double);
	//TODO add copying functionality like so: Vector3(Vector3);
	virtual ~Vector3();

	static double distance(Vector3, Vector3);
	static Vector3 direction(Vector3, Vector3);
	static Vector3 normalize(Vector3);
	double getx()  {return x;	  }
	double gety()  {return y;     }
	double getz()  {return z;     }
	double getLen(){return length;}
	void setAll(double);
	void set(double, double, double);
	void setLength(double l){length = l;}
	void calcLength();
	void print();

	Vector3 operator*(const double& mag)
	{
		Vector3 retVector;
		retVector.x = this->x*mag;
		retVector.y = this->y*mag;
		retVector.z = this->z*mag;
		return retVector;
	}
	Vector3 operator/(const double& mag)
	{
		Vector3 retVector;
		retVector.x = this->x/mag;
		retVector.y = this->y/mag;
		retVector.z = this->z/mag;
		return retVector;
	}
	Vector3 operator-(const Vector3& v)
	{
		Vector3 retVector;
		retVector.x = this->x-v.x;
		retVector.y = this->y-v.y;
		retVector.z = this->z-v.z;
		return retVector;
	}
	Vector3 operator+(const Vector3& v)
	{
		Vector3 retVector;
		retVector.x = this->x+v.x;
		retVector.y = this->y+v.y;
		retVector.z = this->z+v.z;
		return retVector;
	}
	/*std::ostream& operator<<(char*& string)
	{
		std::ostream& os();
		//os.write("", sizeof('\0'));
	    return os << &string << "(" << this->x << "," << this->y << "," << this->z << ")\n";
	}
	std::ostream& operator<<(std::ostream os)
	{
		return os << "(" << this->x << "," << this->y << "," << this->z << ")\n";
	}*/
};

#endif /* VECTOR3_H_ */
