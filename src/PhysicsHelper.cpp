//
// Created by jpaavola on 4/4/2018.
//

#include "PhysicsHelper.h"
using namespace std::chrono;

//TODO: Make sure the translated points are passed into this function, not the basic vertex positions
Collision* PhysicsHelper::gjk_intersection(PhysicsObject *A, PhysicsObject *B) {
	//Start timer
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::vector<glm::vec3> simplex;
	glm::vec3 dir = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec3 a;
	//0ms
	auto A_verts = A->getVertices();
	auto B_verts = B->getVertices();
	milliseconds end1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "Stop 1: " << end1.count() - start.count() << std::endl;
	simplex.push_back(gjk_support(dir, A_verts, B_verts));
	dir = -simplex[0];
	int count = 0;
	milliseconds end2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "Stop 2: " << end2.count() - start.count() << std::endl;
	while(true){
		count++;
		a = gjk_support(dir, A_verts, B_verts);
		//TODO: Check that this is correct
		if(PhysicsHelper::dot(a, dir) < 0.0f) {
			std::cout << "dot(a, dir) < 0.0f after " << count << " iterations.\n";
			//Print duration
			milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			std::cout << "PhysicsHelper::gjk_intersection() took " << end.count() - start.count() << "ms\n";
			return nullptr;
		}
		simplex.push_back(a);
		if(gjk_doSimplex(simplex, dir)) {
			//Print duration
			milliseconds end = duration_cast<milliseconds>(
					system_clock::now().time_since_epoch()
			);
			std::cout << "PhysicsHelper::gjk_intersection() took " << end.count()-start.count() << "ms\n";
			//TODO: Figure out how to get intersection information
			return new Collision(A, B, glm::vec3());
		}
	}
}

/**
 * Gets the point farthest along a direction vector in the "Minkowski Difference"
 * @param dir the direction vector to use
 * @param a_i the set of points to find the maximum of in dir
 * @param b_j the set of points to find the maximum of in -dir
 * @return the point in the "Minkowski Difference" farthest along the direction vector
 */
glm::vec3 PhysicsHelper::gjk_support(glm::vec3 &dir, std::vector<glm::vec3> a_i, std::vector<glm::vec3> b_j) {
	//Start timer
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	//Get furthest point along dir
	glm::vec3 max_a = static_cast<glm::fvec3>(glm::dot(dir, a_i[0]));
	for (int i = 1; i < a_i.size(); ++i) {
		max_a = glm::max(max_a, glm::dot(dir, a_i[i]));
	}
	//Get furthest point along -dir
	glm::vec3 max_b = static_cast<glm::fvec3>(glm::dot(-dir, b_j[0]));
	for(int j = 1; j < b_j.size(); ++j) {
		max_b = glm::max(max_b, glm::dot(-dir, b_j[j]));
	}
	//Print duration
	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "PhysicsHelper::gjk_support() took " << end.count()-start.count() << "ms\n";
	//Return difference
	return max_a - max_b;
}

bool PhysicsHelper::gjk_doSimplex(std::vector<glm::vec3> &simplex, glm::vec3 &dir) {
	//Start timer
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	if(simplex.size()==2){
		gjk_doSimplexLine(simplex, dir);
	} else if(simplex.size()==3) {
		gjk_doSimplexTriangle(simplex, dir);
	} else if(simplex.size()==4) {
		if(gjk_doSimplexTetrahedron(simplex, dir)) {
			//Print duration
			milliseconds end = duration_cast<milliseconds>(
					system_clock::now().time_since_epoch()
			);
			std::cout << "PhysicsHelper::gjk_doSimplex() took " << end.count()-start.count() << "ms\n";
			return true;
		}
	} else {
		std::cout << "gjk_doSimplex() was passed a simplex array with size " << simplex.size() << std::endl;
	}
	//Print duration
	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "PhysicsHelper::gjk_doSimplex() took " << end.count()-start.count() << "ms\n";
	return false;
}

bool PhysicsHelper::SAME_DIR(glm::vec3 vec, glm::vec3 ao) {
	//return glm::dot(vec, ao) > 0.0f;
	return PhysicsHelper::dot(vec, ao) > 0.0f;
}

void PhysicsHelper::gjk_doSimplexTriangle(std::vector<glm::vec3> &simplex, glm::vec3 &dir) {
	glm::vec3 C = simplex[0];
	glm::vec3 B = simplex[1];
	glm::vec3 A = simplex[2];
	glm::vec3 AB = simplex[0] - simplex[1];
	glm::vec3 ABC = glm::cross(C-A, B-A);
	if(SAME_DIR(glm::cross(ABC, C-A), -A)) {
		if(SAME_DIR(C-A, -A)) {
			//Return simplex of AC by removing B
			simplex.clear();
			simplex.push_back(A);
			simplex.push_back(C);
			dir = glm::cross(glm::cross(C-A, -A), C-A);
		}
		else {
			if(SAME_DIR(AB, -A)) {
				//Return simplex of AB
				simplex.clear();
				simplex.push_back(A);
				simplex.push_back(B);
				//Return normal of AB that points to AO
				dir = glm::cross(glm::cross(AB, -A), AB);
			}else {
				std::cout << "A is closest to origin in gjk_doSimplex() for the 2-simplex case." << std::endl;
				//Return simplex of A
				simplex.erase(simplex.begin());
				//Return AO
				dir = -A;
			}
		}
	} else {
		if(SAME_DIR(glm::cross(AB, ABC), -A)) {
			if(SAME_DIR(AB, -A)) {
				//Return simplex of AB
				simplex.clear();
				simplex.push_back(A);
				simplex.push_back(B);
				//Return normal of AB that points to AO
				dir = glm::cross(glm::cross(AB, -A), AB);
			}else {
				std::cout << "A is closest to origin in gjk_doSimplex() for the 2-simplex case." << std::endl;
				//Return simplex of A
				simplex.erase(simplex.begin());
				//Return AO
				dir = -A;
			}
		} else {
			if(SAME_DIR(ABC, -A)) {
				//Return ABC simplex
				simplex.clear();
				simplex.push_back(A);
				simplex.push_back(B);
				simplex.push_back(C);
				//Return "up" normal of triangle
				dir = ABC;
			} else {
				//Return ACB simple
				simplex.clear();
				simplex.push_back(A);
				simplex.push_back(C);
				simplex.push_back(B);
				//Return "down" normal of the triangle
				dir = -ABC;
			}
		}
	}
}

/**
 *
 * @param simplex
 * @param dir
 * @return True if origin is in tetrahedron, otherwise false
 */
//TODO: Rewrite this based on geometry and not guesswork
bool PhysicsHelper::gjk_doSimplexTetrahedron(std::vector<glm::vec3> &simplex, glm::vec3 &dir) {
	glm::vec3 D = simplex[0];
	glm::vec3 C = simplex[1];
	glm::vec3 B = simplex[2];
	glm::vec3 A = simplex[3];
	glm::vec3 ABC = glm::cross(C-A, B-A);
	glm::vec3 ABD = glm::cross(D-A, B-A);
	glm::vec3 BCD = glm::cross(C-D, B-D);
	glm::vec3 ACD = glm::cross(C-A, D-A);
	if(SAME_DIR(ABC, -A)) {
		simplex.clear();
		simplex.push_back(C);
		simplex.push_back(B);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(ABD,  -A)) {
		simplex.clear();
		simplex.push_back(D);
		simplex.push_back(B);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(BCD, -A)) {
		simplex.clear();
		simplex.push_back(D);
		simplex.push_back(C);
		simplex.push_back(B);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(ACD, -A)) {
		simplex.clear();
		simplex.push_back(D);
		simplex.push_back(C);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	}else {
		return true;
	}
	return false;
}

void PhysicsHelper::gjk_doSimplexLine(std::vector<glm::vec3> &simplex, glm::vec3 &dir) {
	glm::vec3 AB = simplex[0] - simplex[1];
	glm::vec3 A = simplex[1];
	glm::vec3 B = simplex[0];
	if(SAME_DIR(AB, -A)) {
		//Return simplex of AB
		simplex.clear();
		simplex.push_back(A);
		simplex.push_back(B);
		//Return normal of AB that points to AO
		dir = glm::cross(glm::cross(AB, -A), AB);
	}else {
		std::cout << "A is closest to origin in gjk_doSimplex() for the 2-simplex case." << std::endl;
		//Return simplex of A
		simplex.erase(simplex.begin());
		//Return AO
		dir = -A;
	}
}

float PhysicsHelper::dot(glm::vec3 a, glm::vec3 b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}






