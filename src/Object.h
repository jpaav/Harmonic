/*
 * Object.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include "GL/glew.h"
#include "Material.h"
#include "Camera.h"
#include "Transform.h"
//#include "Viewport.h"
#include "glm/glm.hpp"
#include <vector>

class Object {
protected:
	Material *m_mat;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	int triCount;
	Camera *m_cam;
	//GLuint simpleShader;
	Transform transform;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
public:
	virtual void draw();
	void drawEdges();
	Object() {}
	Object(Camera* camera, Material* material);
	Object(const char* objPath, Camera* camera, Material* material);
	//Object(GLfloat[], GLfloat[], int, Material, GLuint*, Camera*);
	virtual ~Object();

	void setObjectData(const char* objPath);
	void setLocation(float x, float y, float z);
	void setLocation(glm::vec3 xyz);
	void setRotation(float x, float y, float z);
	void setRotation(glm::vec4 xyzw);
	void setScale(float x, float y, float z);
	void setScale(glm::vec3 xyz);
	void setMaterial(Material* material);
};

#endif /* OBJECT_H_ */
