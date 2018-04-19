//
// Created by jpaavola on 3/6/2018.
//

#ifndef HARMONIC_COLLISION_H
#define HARMONIC_COLLISION_H


#include "Object.h"

class Collision {
public:
	Object *collider;
	Object *collidee;
	glm::vec3 intersection;

	Collision(Object *collider, Object *collidee, glm::vec3 intersection);

	std::string toString();
};


#endif //HARMONIC_COLLISION_H
