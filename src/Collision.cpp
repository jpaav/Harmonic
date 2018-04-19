//
// Created by jpaavola on 3/6/2018.
//

#include "Collision.h"

Collision::Collision(Object *collider, Object *collidee, glm::vec3 intersection) : collider(collider), collidee(collidee), intersection(intersection) {}
std::string Collision::toString() {
	return std::string(collider->getName() + " collides with " + collidee->getName());
}
