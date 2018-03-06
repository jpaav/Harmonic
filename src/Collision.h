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

	Collision(Object *collider, Object *collidee);

	std::string toString();
};


#endif //HARMONIC_COLLISION_H
