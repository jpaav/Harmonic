/*
 * Color.h
 *
 *  Created on: Dec 11, 2016
 *      Author: Joseph
 */

#ifndef COLOR_H_
#define COLOR_H_
#include "glm/glm.hpp"

class Color {
private:
	glm::vec3 rgb;
	glm::vec3 hsv;
public:
	float r(){return rgb.r;}
	float g(){return rgb.g;}
	float b(){return rgb.b;}

	float h(){return hsv.x;}
	float s(){return hsv.y;}
	float v(){return hsv.z;}

	glm::vec3 RGBtoHSV(glm::vec3 rgb);
	glm::vec3 getRGB(){return rgb;}
	glm::vec3 getHSV(){return hsv;}
	glm::vec3 getNormalized();
	void setRGB(float r, float b, float g);
	void setHSV(float h, float s, float v);
	Color();
	Color(float r, float g, float b);
	virtual ~Color();
};

#endif /* COLOR_H_ */
