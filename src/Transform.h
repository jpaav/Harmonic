#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <iostream>

class Transform
{
public:
	//Basic Transform
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	//Physics based
	glm::vec3 velocity;
	glm::vec3 acceleration;
	std::vector<glm::vec3> localForces;
	//Methods
	void print();
	//Constructors
	Transform();
	~Transform();
};

