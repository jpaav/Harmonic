//
// Created by jpaavola on 3/6/2018.
//

#include "Collision.h"

Collision::Collision(Object *collider, Object *collidee) : collider(collider), collidee(collidee) {}
std::string Collision::toString() {
	return std::string(collider->getName() + " collides with " + collidee->getName());
}
