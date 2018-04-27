//
// Created by jpaavola on 4/4/2018.
//

#include "PhysicsHelper.h"

using namespace std::chrono;

#define GJK_MAX_ITERATIONS 64

Collision* PhysicsHelper::gjk_intersection(PhysicsObject *A, PhysicsObject *B, GLFWwindow* window, bool draw) {
	//Start timer
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::vector<glm::vec3> simplex;
	glm::vec3 a;
	glm::vec3 dir = B->obbTree->getCenter() - A->obbTree->getCenter();//glm::vec3(1.0f,0.0f,0.0f);
	//0ms
	auto A_verts = A->getVertices();
	auto B_verts = B->getVertices();
	milliseconds end1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "Stop 1: " << end1.count() - start.count() << std::endl;
	a = gjk_support(dir, A_verts, B_verts);
	simplex.push_back(a);
	dir = -a;
	int count = 0;
	milliseconds end2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "Stop 2: " << end2.count() - start.count() << std::endl;
	int lastState = GLFW_RELEASE;
	while(count < GJK_MAX_ITERATIONS){
		//Draw
		if(draw) {
			gjk_drawSimplex(simplex, A, window);
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && lastState == GLFW_RELEASE){
				lastState = GLFW_PRESS;
			} else if(glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE && lastState == GLFW_PRESS) {
				lastState = GLFW_RELEASE;
				continue;
			} else {
				continue;
			}
		}
		count++;
		a = gjk_support(dir, A_verts, B_verts);
		//TODO: Check that this is correct
		if(PhysicsHelper::dot(a, dir) <= 0.0f) {
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
	return nullptr;
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
	//TODO: store index and retreive correct vertice at return statement
	int max_a_index = 0, max_b_index = 0;
	for (int i = 1; i < a_i.size(); ++i) {
		if(PhysicsHelper::dot(dir, a_i[max_a_index]) < PhysicsHelper::dot(dir, a_i[i])) {
			max_a_index = i;
		}
	}
	//Get furthest point along -dir
	for(int j = 1; j < b_j.size(); ++j) {
		if(PhysicsHelper::dot(-dir, b_j[max_b_index]) < PhysicsHelper::dot(-dir, b_j[j])) {
			max_b_index = j;
		}
	}
	//Print duration
	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::cout << "PhysicsHelper::gjk_support() took " << end.count()-start.count() << "ms\n";
	//Return difference
	return a_i[max_a_index] - b_j[max_b_index];
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
	glm::vec3 AB = B-A;
	glm::vec3 AC = C-A;
	glm::vec3 ABC = glm::cross(AB, AC);
	if(SAME_DIR(glm::cross(ABC, AC), -A)) {
		if(SAME_DIR(AC, -A)) {
			std::cout << "CA is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
			//Return simplex of CA by removing B
			simplex.clear();
			simplex.push_back(C);
			simplex.push_back(A);
			dir = glm::cross(glm::cross(AC, -A), AC);
		}
		else {
			if(SAME_DIR(AB, -A)) {
				std::cout << "BA is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return simplex of BA
				simplex.clear();
				simplex.push_back(B);
				simplex.push_back(A);
				//Return normal of AB that points to AO
				dir = glm::cross(glm::cross(AB, -A), AB);
			}else {
				std::cout << "A is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return simplex of A
				simplex.clear();
				simplex.push_back(A);
				//Return AO
				dir = -A;
			}
		}
	} else {
		if(SAME_DIR(glm::cross(AB, ABC), -A)) {
			if(SAME_DIR(AB, -A)) {
				std::cout << "BA is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return simplex of AB
				simplex.clear();
				simplex.push_back(B);
				simplex.push_back(A);
				//Return normal of AB that points to AO
				dir = glm::cross(glm::cross(AB, -A), AB);
			}else {
				std::cout << "A is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return simplex of A
				simplex.clear();
				simplex.push_back(A);
				//Return AO
				dir = -A;
			}
		} else {
			if(SAME_DIR(ABC, -A)) {
				std::cout << "CBA is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return CBA simplex
				simplex.clear();
				simplex.push_back(C);
				simplex.push_back(B);
				simplex.push_back(A);
				//Return "up" normal of triangle
				dir = ABC;
			} else {
				std::cout << "BCA is closest to origin in gjk_doSimplex() for the 3-simplex case." << std::endl;
				//Return CBA simple
				simplex.clear();
				simplex.push_back(C);
				simplex.push_back(B);
				simplex.push_back(A);
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
		std::cout << "CBA is closest to origin in gjk_doSimplex() for the 4-simplex case." << std::endl;
		simplex.clear();
		simplex.push_back(C);
		simplex.push_back(B);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(ABD,  -A)) {
		std::cout << "DBA is closest to origin in gjk_doSimplex() for the 4-simplex case." << std::endl;
		simplex.clear();
		simplex.push_back(D);
		simplex.push_back(B);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(BCD, -A)) {
		std::cout << "CDB is closest to origin in gjk_doSimplex() for the 4-simplex case." << std::endl;
		simplex.clear();
		simplex.push_back(C);
		simplex.push_back(D);
		simplex.push_back(B);
		gjk_doSimplexTriangle(simplex, dir);
	} else if(SAME_DIR(ACD, -A)) {
		std::cout << "DCA is closest to origin in gjk_doSimplex() for the 4-simplex case." << std::endl;
		simplex.clear();
		simplex.push_back(D);
		simplex.push_back(C);
		simplex.push_back(A);
		gjk_doSimplexTriangle(simplex, dir);
	}else {
		std::cout << "Tetrahedron contains origin in gjk_doSimplex() for the 4-simplex case." << std::endl;
		return true;
	}
	return false;
}

void PhysicsHelper::gjk_doSimplexLine(std::vector<glm::vec3> &simplex, glm::vec3 &dir) {
	glm::vec3 A = simplex[1];
	glm::vec3 B = simplex[0];
	glm::vec3 AB = B-A;

	if(SAME_DIR(AB, -A)) {
		std::cout << "BA is closest to origin in gjk_doSimplex() for the 2-simplex case." << std::endl;
		//Return simplex of BA
		simplex.clear();
		simplex.push_back(B);
		simplex.push_back(A);
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

float PhysicsHelper::dot(glm::vec3 a, float b_x, float b_y, float b_z) {
	return a.x*b_x + a.y*b_y + a.z*b_z;
}

void PhysicsHelper::gjk_drawSimplex(std::vector<glm::vec3> &simplex, PhysicsObject *object, GLFWwindow *window) {
	auto *vp = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Enables textures (Should it be outside main loop?)
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	//Depth
	glDepthFunc(GL_LESS);
	//Define vieport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	if(simplex.size()==1) {
		object->addPoint(simplex[0]);
	} else if(simplex.size()==2){
		object->addLine(simplex[0], simplex[1]);
	} else if(simplex.size()==3) {
		object->addTriangle(simplex[0], simplex[1], simplex[2]);
	} else if(simplex.size()==4) {
		object->addTetrahedron(simplex[0], simplex[1], simplex[2], simplex[3]);
	}
	else {
		std::cout << "gjk_drawSimplex() was passed a simplex array with size " << simplex.size() << std::endl;
	}
	vp->drawPointsAndLines();

	//Update render view by swapping front and back buffers
	glfwSwapBuffers(window);
	//Poll for and process events
	glfwPollEvents();
}






