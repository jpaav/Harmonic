/*
 * Color.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: Joseph
 */

#include "Color.h"
#include "glm/glm.hpp"

Color::Color() {
	rgb = glm::vec3(1.0f, 1.0f, 1.0f);
	hsv = glm::vec3(1.0f, 0.0f, 1.0f);
}

Color::Color(float r, float g, float b)
{
	rgb = glm::vec3(r, g, b);
	hsv = RGBtoHSV(rgb);
}
Color::~Color() {
	// TODO Auto-generated destructor stub
}

glm::vec3 Color::RGBtoHSV(glm::vec3 rgb)
{
	float h=1.0f,s=0.0f,v=1.0f;
	rgb.r /= 255;
	rgb.g /= 255;
	rgb.b /= 255;
	float max = glm::max(rgb.r, glm::max(rgb.g, rgb.b));
	float min = glm::min(rgb.r, glm::min(rgb.g, rgb.b));
	float delta = max-min;

	if(rgb.r > rgb.g && rgb.r >rgb.b){
		h = (1/6)*glm::mod((rgb.g-rgb.b)/delta, 6.0f);
	}
	else if(rgb.g > rgb.r && rgb.g >rgb.b){
		h = (1/6)*(((rgb.b-rgb.r)/delta)+2.0f);
	}
	else if (rgb.b > rgb.g && rgb.b >rgb.g){
		h = (1/6)*(((rgb.r-rgb.g)/delta)+4.0f);
	}
	if(max!=0){
		s = delta/max;
	}
	v = max;
	return glm::vec3(h,s,v);
}
