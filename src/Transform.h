#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

class Transform
{
public:
	//Basic Transform
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	//Physics based
	glm::vec3 velocity;
	glm::vec3 acceleration;
	std::vector<glm::vec3> localForces;
	//Methods
	void print();
	glm::vec3 getEulerRotation() { return glm::degrees(glm::eulerAngles(rotation)); }
	//Constructors
	Transform();
	~Transform();
};

