/*
 * PhysicsObject.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: josephpaavola
 */

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Camera *camera, Material *material) : Object(camera, material){
	
	mass = 1.0;
	isPinned = false;

}

PhysicsObject::~PhysicsObject() {
	mass = 1.0;
	isPinned = false;
}

void PhysicsObject::update(double deltaT, std::vector<glm::vec3> globalForces) {
	//Stop update if object doesn't move
	//This may be changed later when collisions are added
	if (isPinned) { return; }
	//Combine global and local forces
	std::vector<glm::vec3> forces;
	forces.push_back(combineForces(globalForces));
	forces.push_back(combineForces(transform.localForces));
	//Apply forces
	applyForce(deltaT, combineForces(forces));
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
	std::cout << p_x << ", " << p_y << ", " << p_z << std::endl;
	//transform.print();
}


