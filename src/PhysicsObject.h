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
#include "OBBTree.h"
#include "Collision.h"

class PhysicsObject: public Object {
public:
	//Constructors and Destructors
	PhysicsObject(Camera *camera, Material *material, GLuint shader);

	~PhysicsObject() override;
	//Methods
	Collision* updateCollisions(PhysicsObject *otherObject);
	void updateForces(double deltaT, std::vector<glm::vec3> globalForces);
	void applyForce(double deltaT, glm::vec3 force);
	void setMass(float mass) { this->mass = mass; }
	void draw() override;
	void setObjectData(const char* objPath) override ;
	bool isColliding() { return colliding; };
	//Instance variables
	bool isPinned;
	GLuint infoShader;
	OBBTree *obbTree;
	//Static functions
	glm::vec3 static combineForces(std::vector<glm::vec3> forces)
	{
		glm::vec3 forceSum = glm::vec3(0.0f, 0.0f, 0.0f);
		for (auto &force : forces) {
			forceSum.x += force.x;
			forceSum.y += force.y;
			forceSum.z += force.z;
		}
		return forceSum;
	}
private:
	float mass;
	AABB aabb;
	bool colliding;
};

#endif /* PHYSICSOBJECT_H_ */
