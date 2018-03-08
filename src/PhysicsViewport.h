#pragma once
#include "Viewport.h"
#include "PhysicsObject.h"
#include "glm/glm.hpp"
#include "Collision.h"
#include <vector>

enum UpdateState {
	WAIT,
	SINGLE_FRAME,
	CONTINUOUS
};

class PhysicsViewport :
	public Viewport
{
public:
	//Public instance variables
	std::vector<glm::vec3> globalForces;
	GLuint infoShader;
	std::vector<Collision*> collisions;
	UpdateState update_state;
	long simFrame;
	//Constructors and Destructors
	PhysicsViewport(GLFWwindow *window, int width, int height, Camera *camera, GLuint shader);
	PhysicsViewport(GLFWwindow *window, int width, int height, GLuint shader);
	~PhysicsViewport() override;
	//Public methods
	void addGlobalForce(glm::vec3 force);
	PhysicsObject* addPhysicsObject(const char * materialName);
	void updatePhysics();
};

