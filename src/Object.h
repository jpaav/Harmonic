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
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "GLFunctions.h"

class Object {
protected:
	std::string name;
	Material *m_mat;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	GLuint transformFeedbackBuffer;
	int triCount;
	Camera *m_cam;
	//GLuint simpleShader;
	Transform transform;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices_worldspace;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> lines;
	std::vector<glm::vec3> points;
public:
	virtual void draw();
	void drawEdges();
	Object(Camera* camera, Material* material);
	Object(const char* objPath, Camera* camera, Material* material);
	//Object(GLfloat[], GLfloat[], int, Material, GLuint*, Camera*);
	virtual ~Object();

	virtual void setObjectData(const char* objPath);
	void setLocation(float x, float y, float z);
	void setLocation(glm::vec3 xyz);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setScale(glm::vec3 xyz);
	std::vector<glm::vec3> & getVertices();

	Material *getMaterial() const {
		return m_mat;
	}

	void setMaterial(Material *m_mat) {
		Object::m_mat = m_mat;
	}

	const std::string &getName() const {
		return name;
	}

	//TODO: use :: instead of -> for "this"
	void setName(const std::string &name) {
		Object::name = name;
	}
	void setName(const char* &name) {
		Object::name = std::string(name);
	}

	Transform &getTransform() {
		return transform;
	}


	virtual void getPointsAndLines(std::vector<glm::vec3> *allLines) {
		for(auto point : points) {
			lines.emplace_back(point.x, point.y-1, point.z);
			lines.emplace_back(point.x, point.y+1, point.z);
			lines.emplace_back(point.x-1, point.y, point.z);
			lines.emplace_back(point.x+1, point.y, point.z);
			lines.emplace_back(point.x, point.y, point.z-1);
			lines.emplace_back(point.x, point.y, point.z+1);
		}
		allLines->insert(allLines->end(), lines.begin(), lines.end());
	}

	void addLine(glm::vec3 a, glm::vec3 b);
	void addTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	void addTetrahedron(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	void addPoint(glm::vec3 a);
};

#endif /* OBJECT_H_ */
