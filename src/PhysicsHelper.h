//
// Created by jpaavola on 4/4/2018.
//

#ifndef HARMONIC_PHYSICSHELPER_H
#define HARMONIC_PHYSICSHELPER_H


#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "Collision.h"
#include "PhysicsObject.h"
#include <chrono>

class PhysicsHelper {
public:
	static Collision* gjk_intersection(PhysicsObject *A, PhysicsObject *B);
	static float dot(glm::vec3 a, glm::vec3 b);
private:
	static glm::vec3 gjk_support(glm::vec3 &dir, std::vector<glm::vec3> a_i, std::vector<glm::vec3> b_j);
	static bool gjk_doSimplex(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	static void gjk_doSimplexTriangle(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	static bool gjk_doSimplexTetrahedron(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	static void gjk_doSimplexLine(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	static bool SAME_DIR(glm::vec3 vec, glm::vec3 ao);

};


#endif //HARMONIC_PHYSICSHELPER_H
