//
// Created by jpaavola on 2/7/2018.
//

#ifndef HARMONIC_OBBTREE_H
#define HARMONIC_OBBTREE_H

#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GLFunctions.h"
#include "Transform.h"
#include <Eigen/Eigenvalues>
#include <Eigen/Core>

typedef std::vector<glm::vec3> Triangle;

class OBBTree {
public:
	//Constructors and Destructors
	OBBTree(std::vector<glm::vec3> *vertices);
	virtual ~OBBTree();
	//Methods
	std::vector<Triangle> loadTriangles(std::vector<glm::vec3> vertices);
	void findBounds(glm::mat3 basis, std::vector<glm::vec3> *vertices);
	void draw(GLuint shader, Transform objTransform, Camera *camera);
	//Instance variables
	glm::vec3 meanMatrix;
	glm::mat3 covarianceMatrix;
	glm::mat3 eigenVectors;
	glm::mat4 scaleMatrix;
	GLuint vertexBuffer;
	std::vector<glm::vec3> boxVertices;
};


#endif //HARMONIC_OBBTREE_H
