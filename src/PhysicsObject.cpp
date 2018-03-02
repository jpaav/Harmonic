/*
 * PhysicsObject.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: josephpaavola
 */

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Camera *camera, Material *material, GLuint shader) : Object(camera, material){
	infoShader = shader;
	mass = 1.0;
	isPinned = false;
	obbTree = nullptr;
}

PhysicsObject::~PhysicsObject() {
	delete(obbTree);
}

void PhysicsObject::updateCollisions(double deltaT, PhysicsObject *otherObject) {
	//TODO: change otherObject name to be less terrible
	//Get all 15 axis of separation
	//TODO: there should be 15 but we're missing 3, not sure which
	std::vector<glm::vec3> L;
	L.push_back(obbTree->covarianceMatrix[0]);
	L.push_back(obbTree->covarianceMatrix[1]);
	L.push_back(obbTree->covarianceMatrix[2]);
	L.push_back(otherObject->obbTree->covarianceMatrix[0]);
	L.push_back(otherObject->obbTree->covarianceMatrix[1]);
	L.push_back(otherObject->obbTree->covarianceMatrix[2]);
	L.push_back(obbTree->covarianceMatrix[0] * otherObject->obbTree->covarianceMatrix[0]);
	L.push_back(obbTree->covarianceMatrix[0] * otherObject->obbTree->covarianceMatrix[1]);
	L.push_back(obbTree->covarianceMatrix[0] * otherObject->obbTree->covarianceMatrix[2]);
	L.push_back(obbTree->covarianceMatrix[1] * otherObject->obbTree->covarianceMatrix[1]);
	L.push_back(obbTree->covarianceMatrix[1] * otherObject->obbTree->covarianceMatrix[2]);
	L.push_back(obbTree->covarianceMatrix[2] * otherObject->obbTree->covarianceMatrix[2]);

	glm::vec3 A = obbTree->meanMatrix;
	glm::vec3 B = otherObject->obbTree->meanMatrix;
	//Half dimensions
	//TODO: to implement this, find the vertex closest to B
	glm::vec3 a;
	glm::vec3 b;
	//Initialize radii
	float r_a=0;
	float r_b=0;
	//TODO vvv ake sure the signs on this are ok vvv
	glm::vec3 T = B - A;

	for (int i = 0; i < 15; ++i) {
		//TODO: procedure:
		//Find r_a and r_b
	}
}

void PhysicsObject::applyForce(double deltaT, glm::vec3 force)
{
	double p_x, p_y, p_z, v_x, v_y, v_z;

	transform.acceleration.x = force.x / mass;
	transform.acceleration.y = force.y / mass;
	transform.acceleration.z = force.z / mass;

	v_x = transform.velocity.x + (deltaT * transform.acceleration.x);
	v_y = transform.velocity.y + (deltaT * transform.acceleration.y);
	v_z = transform.velocity.z + (deltaT * transform.acceleration.z);

	p_x = transform.position.x + (transform.velocity.x*deltaT) + (0.5*transform.acceleration.x*deltaT*deltaT);
	p_y = transform.position.y + (transform.velocity.y*deltaT) + (0.5*transform.acceleration.y*deltaT*deltaT);
	p_z = transform.position.z + (transform.velocity.z*deltaT) + (0.5*transform.acceleration.z*deltaT*deltaT);

	transform.velocity = glm::vec3(v_x, v_y, v_z);
	transform.position = glm::vec3(p_x, p_y, p_z);
}

void PhysicsObject::draw() {
	Object::draw();

	// Update and Draw AABB
	//aabb.draw(infoShader, vertices, this->transform, this->m_cam);
	obbTree->draw(infoShader, this->transform, this->m_cam);
}

void PhysicsObject::setObjectData(const char *objPath) {
	Object::setObjectData(objPath);
	obbTree = new OBBTree(&vertices);
}

void PhysicsObject::updateForces(double deltaT, std::vector<glm::vec3> globalForces) {
	//Stop update if object doesn't move
	if (isPinned) { return; }
	//Combine global and local forces
	std::vector<glm::vec3> forces;
	forces.push_back(combineForces(globalForces));
	forces.push_back(combineForces(transform.localForces));
	//Apply forces
	applyForce(deltaT, combineForces(forces));
}

