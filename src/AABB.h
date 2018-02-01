#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	glm::mat4 transform;
	float minx, miny, minz;
	float maxx, maxy, maxz;
	GLuint aabbBuffer;
	//Methods
	void update(std::vector<glm::vec3> objVertices);
	void draw(GLuint shader, Transform tranform, Camera* cam);
};

