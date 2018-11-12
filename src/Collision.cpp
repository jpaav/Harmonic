//
// Created by jpaavola on 3/6/2018.
//

#include "Collision.h"

Collision::Collision(PhysicsObject *A, PhysicsObject *B, glm::vec3 contact_n_A, glm::vec3 contact_n_B, glm::vec3 intersection_A, glm::vec3 intersection_B) : A(A), B(B), contact_n_A(contact_n_A), contact_n_B(contact_n_B), intersection_A(intersection_A), intersection_B(intersection_B) {}
std::string Collision::toString() {
	return std::string(A->getName() + " collides with " + B->getName() + "\nalong normal " + glm::to_string(contact_n_A));
}
