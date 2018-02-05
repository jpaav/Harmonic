/*
 * PhysicsObject.h
 *
 *  Created on: Nov 2, 2017
 *      Author: josephpaavola
 */

#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_

#include "Object.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "AABB.h"

class PhysicsObject: public Object {
public:
	//Constructors and Destructors
	PhysicsObject(Camera *camera, Material *material);
	virtual ~PhysicsObject();
	//Methods
	void update(double deltaT, std::vector<glm::vec3> globalForces);
	void applyForce(double deltaT, glm::vec3 force);
	void setMass(double mass) { this->mass = mass; }
	void draw();
	//Instance variables
	bool isPinned;
	//Static functions
	glm::vec3 static combineForces(std::vector<glm::vec3> forces)
	{
		glm::vec3 forceSum = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int i=0; i<forces.size(); i++)
		{
			forceSum.x += forces[i].x;
			forceSum.y += forces[i].y;
			forceSum.z += forces[i].z;
		}
		return forceSum;
	}
private:
	double mass;
	AABB aabb;
};

#endif /* PHYSICSOBJECT_H_ */
