#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/ext.hpp>
#include <vector>
#include "Transform.h"
#include "Camera.h"

class AABB
{
public:
	//-structors
	AABB();
	~AABB();
	//Instance variables
	glm::vec3 size;
	glm::vec3 center;
	glm::mat4 m_transform;
	float minx, miny, minz;
	float maxx, maxy, maxz;
	GLuint aabbBuffer;
	GLfloat vertices[108] = {
			-1.0f, -1.0f, -1.0f, // triangle 1 : begin
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f, -1.0f, // triangle 2 : begin
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, // triangle 2 : end
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
	};
	//Methods
	void update(std::vector<glm::vec3> objVertices, Transform objTransform);
	void draw(GLuint shader, std::vector<glm::vec3> objVertices, Transform objTranform, Camera* cam);
};

