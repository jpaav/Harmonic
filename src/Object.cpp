/*
 * Object.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */
#include <iostream>
#include "Object.h"
#include "Material.h"
#include "Camera.h"
#include "GLFunctions.h"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stdio.h"

void _glAssert(const char * info, int line) {
	char const* sz_GL_INVALID_ENUM = "GL_INVALID_ENUM";
	char const* sz_GL_INVALID_VALUE = "GL_INVALID_VALUE";
	char const* sz_GL_INVALID_OPERATION = "GL_INVALID_OPERATION";
	char const* sz_GL_OUT_OF_MEMORY = "GL_OUT_OF_MEMORY";
	GLenum result;
	if ((result = glGetError()) != GL_NO_ERROR) {
		const char * sz_result = NULL;
		switch (result) {
		case GL_INVALID_ENUM:
			sz_result = sz_GL_INVALID_ENUM;
			break;
		case GL_INVALID_VALUE:
			sz_result = sz_GL_INVALID_VALUE;
			break;
		case GL_INVALID_OPERATION:
			sz_result = sz_GL_INVALID_OPERATION;
			break;
		case GL_OUT_OF_MEMORY:
			sz_result = sz_GL_OUT_OF_MEMORY;
			break;
		}

		_ASSERT(sz_result, info, line);
	}
}

#define glAssert() \
    _glAssert(__FILE__, __LINE__);
Object::Object(const char* objPath, Camera* camera, Material *material) {
	m_cam = camera;
	triCount = -1;
	m_mat = material;
	GLuint ub;
	glGenBuffers(1, &ub);
	uvBuffer = ub;
	glBindBuffer(GL_ARRAY_BUFFER, ub);
	GLuint vb;
	glGenBuffers(1, &vb);
	vertexBuffer = vb;
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	setObjectData(objPath);
}
Object::Object(Camera* camera, Material *material){
	m_cam = camera;
	triCount = -1;
	m_mat = material;
	GLuint ub;							//Necessary?
	glGenBuffers(1, &ub);
	uvBuffer = ub;						//Necessary?
	glBindBuffer(GL_ARRAY_BUFFER, ub);	//Necessary?
	GLuint vb;							//Necessary?
	glGenBuffers(1, &vb);
	vertexBuffer = vb;					//Necessary?
	glBindBuffer(GL_ARRAY_BUFFER, vb);	//Necessary?
}

Object::~Object() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
}

void Object::setObjectData(const char* objPath){
	bool res = loadObj(objPath, &vertices, &uvs, &normals);
	if(res==false){ printf("loadObj returned false"); }

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

	triCount = (int)(vertices.size());
}

void Object::setLocation(float x,float y,float z){
	transform.position = glm::vec3(x,y,z);
}
void Object::setLocation(glm::vec3 newLoc){
	transform.position = newLoc;
}
void Object::setRotation(float x,float y,float z){
	transform.rotation = glm::vec4(x,y,z,0.0f);
}
void Object::setRotation(glm::vec4 xyzw){
	transform.rotation = xyzw;
}
void Object::setScale(float x,float y,float z){
	transform.scale = glm::vec3(x,y,z);
}
void Object::setScale(glm::vec3 newScale){
	transform.scale = newScale;
}

void Object::draw(){	//CHECK WHETHER OBJECT USES UVS TO SAVE RESOURCES
	//Run shader
	glUseProgram(m_mat->getShader());

	//Generating the MVP matrix
	glm::mat4 LocationMatrix = glm::translate(transform.position);
	glm::mat4 ScalingMatrix = glm::scale(transform.scale);
	glm::mat4 MVPmatrix = m_cam->dynamicCameraMatrix() * LocationMatrix/* * RotaionMatrix*/ * ScalingMatrix;

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc =	glGetUniformLocation(m_mat->getShader(), "MVP");
	if(m_mat->getTexture() != NULL){
		GLuint TextureLoc =	glGetUniformLocation(m_mat->getShader(), "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_mat->getTexture());
		glUniform1i(TextureLoc, 0);
	}

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,			//Attribute 0. must match shader layout
		3,			//size
		GL_FLOAT,	//type
		GL_FALSE,	//normalized?
		0,			//stride
		(void*)0	//array buffer offset
	);

	//Add UV Attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1, 			//Attribute 1.
		2,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, triCount); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Object::drawEdges(){
	glUseProgram(m_mat->getShader());

	//Generating the MVP matrix
	glm::mat4 LocationMatrix = glm::translate(transform.position);
	glm::mat4 ScalingMatrix = glm::scale(transform.scale);
	//glm::mat4 RoationMatrix =
	glm::mat4 MVPmatrix = m_cam->dynamicCameraMatrix() * LocationMatrix/* * RotaionMatrix*/ * ScalingMatrix;

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc =	glGetUniformLocation(m_mat->getShader(), "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,			//Attribute 0. must match shader layout
		3,			//size
		GL_FLOAT,	//type
		GL_FALSE,	//normalized?
		0,			//stride
		(void*)0	//array buffer offset
	);

	//Add UV Attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1, 			//Attribute 1.
		2,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, triCount); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
void Object::setMaterial(Material *m){
	//delete m_mat;
	m_mat = m;
}
