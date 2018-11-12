//
// Created by jpaavola on 3/6/2018.
//

#ifndef HARMONIC_COLLISION_H
#define HARMONIC_COLLISION_H


#include "Object.h"
#include "PhysicsObject.h"
#include <glm/ext.hpp>

class PhysicsObject;
class Collision {
public:
	PhysicsObject *A;
	PhysicsObject *B;
	glm::vec3 contact_n_A;
	glm::vec3 contact_n_B;
	glm::vec3 intersection_A;
	glm::vec3 intersection_B;

	Collision(PhysicsObject *A, PhysicsObject *B, glm::vec3 contact_n_A, glm::vec3 contact_n_B, glm::vec3 intersection_A, glm::vec3 intersection_B);

	std::string toString();
};


#endif //HARMONIC_COLLISION_H
