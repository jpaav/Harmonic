#pragma once
#include "Viewport.h"
#include "PhysicsObject.h"
#include "glm/glm.hpp"
#include <vector>
class PhysicsViewport :
	public Viewport
{
public:
	//Public instance variables
	std::vector<glm::vec3> globalForces;
	//Constructors and Destructors
	PhysicsViewport(GLFWwindow *window, int width, int height, Camera *camera);
	PhysicsViewport(GLFWwindow *window, int width, int height);
	~PhysicsViewport();
	//Public methods
	void addGlobalForce(glm::vec3 force);
	PhysicsObject* addPhysicsObject(char * materialName);
	void updatePhysics();
};

