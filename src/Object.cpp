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
#include <crtdbg.h>
#include <cstdio>
#include <glm/gtc/type_ptr.hpp>

//Never used
/*void _glAssert(const char * info, int line) {
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
    */

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
	GLuint tfb;
	glGenBuffers(1, &tfb);
	transformFeedbackBuffer = tfb;
	glBindBuffer(GL_TRANSFORM_FEEDBACK, tfb);
	GLuint nb;
	glGenBuffers(1, &nb);
	normalBuffer = nb;
	glBindBuffer(GL_ARRAY_BUFFER, nb);
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
	GLuint tfb;
	glGenBuffers(1, &tfb);
	transformFeedbackBuffer = tfb;
	glBindBuffer(GL_TRANSFORM_FEEDBACK, tfb);
	GLuint nb;
	glGenBuffers(1, &nb);
	normalBuffer = nb;
	glBindBuffer(GL_ARRAY_BUFFER, nb);
}

Object::~Object() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &transformFeedbackBuffer);
}

void Object::setObjectData(const char* objPath){
	bool res = loadObj(objPath, &vertices, &uvs, &normals);
	if(!res){ printf("loadObj returned false"); }

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, transformFeedbackBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), 0, GL_STATIC_READ);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	triCount = (int)(vertices.size());
}

void Object::setLocation(float x,float y,float z){
	transform.position = glm::vec3(x,y,z);
}
void Object::setLocation(glm::vec3 newLoc){
	transform.position = newLoc;
}
void Object::setRotation(float x,float y,float z){
	transform.rotation = glm::quat(glm::vec3(x, y, z));
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
	glm::mat4 RotationMatrix = glm::toMat4(transform.rotation);
	glm::mat4 viewProjMatrix = m_cam->dynamicCameraMatrix();
	glm::mat4 modelMatrix = LocationMatrix * RotationMatrix * ScalingMatrix;

	//Get a handle on the MVP/sampler uniforms
	GLint ModelMatLoc = glGetUniformLocation(m_mat->getShader(), "model");
	GLint ViewProjMatLoc = glGetUniformLocation(m_mat->getShader(), "viewProj");

	//Handle textures
	if(m_mat->getTexture() != NULL){
		GLint TextureLoc =	glGetUniformLocation(m_mat->getShader(), "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_mat->getTexture());
		glUniform1i(TextureLoc, 0);
		//Enable
		GLint TextureEnabledLoc = glGetUniformLocation(m_mat->getShader(), "useTextures");
		glUniform1i(TextureEnabledLoc, 1);
	}else{
		GLint TextureEnabledLoc = glGetUniformLocation(m_mat->getShader(), "useTextures");
		glUniform1i(TextureEnabledLoc, 0);
	}



	GLint ColorLoc = glGetUniformLocation(m_mat->getShader(), "tint");
	GLint ViewPosLoc = glGetUniformLocation(m_mat->getShader(), "viewPos");
	GLint ShininessLoc = glGetUniformLocation(m_mat->getShader(), "shininess");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(ViewProjMatLoc, 1, GL_FALSE, glm::value_ptr(viewProjMatrix));

	//Send Color to shader in uniform variable
	if(ColorLoc != -1){
		glUniform3fv(ColorLoc, 1, glm::value_ptr(m_mat->getColor().getRGB()));
	}

	//Send View Position to shader in uniform variable
	if(ViewPosLoc != -1) {
		glUniform3fv(ViewPosLoc, 1, glm::value_ptr(m_cam->getPos()));
	}

	//Send Shininess to shader in uniform variable
	if(ShininessLoc != -1) {
		glUniform1f(ShininessLoc, m_mat->getShininess());
	}

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,			//Attribute 0.
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

	//Add Normal Attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
			2, 			//Attribute 1.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
	);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, triCount);

	//Get World Space positions for vertices via Transform Feedback
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackBuffer);
	//TODO: line below may be unnecessary
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, transformFeedbackBuffer);
	glBeginTransformFeedback(GL_POINTS);
	glEnable(GL_RASTERIZER_DISCARD);
	glDrawArrays(GL_POINTS, 0, triCount*3);
	glDisable(GL_RASTERIZER_DISCARD);
	glEndTransformFeedback();

	//Disable Vertex Attrib Arrays
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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

std::vector<glm::vec3> & Object::getVertices() {
	auto start = glfwGetTime();
	/*GPU acceleration
	 size_t length = vertices.size()*3;
	GLfloat worldSpaceVertices[length];
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, transformFeedbackBuffer);
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(worldSpaceVertices), worldSpaceVertices);
	//return worldSpaceVertices;
	vertices_worldspace = floatArrayToGLMVector(worldSpaceVertices, length);*/
	glm::mat4 LocationMatrix = glm::translate(transform.position);
	glm::mat4 ScalingMatrix = glm::scale(transform.scale);
	glm::mat4 RotationMatrix = glm::toMat4(transform.rotation);
	glm::mat4 MVPmatrix = LocationMatrix * RotationMatrix * ScalingMatrix;
	vertices_worldspace.clear();
	for (auto vertice : vertices) {
		vertices_worldspace.emplace_back(MVPmatrix * glm::vec4(vertice, 1));
	}
	auto end = glfwGetTime();
	std::cout << "Object::getVertices() took " << 1000*(end-start) << "ms\n";
	return vertices_worldspace;
}

void Object::addLine(glm::vec3 a, glm::vec3 b) {
	lines.push_back(a);
	lines.push_back(b);
}

void Object::addTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(a);
	lines.push_back(c);
	lines.push_back(c);
	lines.push_back(b);
}

void Object::addTetrahedron(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	//Draw Base Triangle
	lines.push_back(a);
	lines.push_back(b);
	lines.push_back(a);
	lines.push_back(c);
	lines.push_back(c);
	lines.push_back(b);
	//Draw Rest
	lines.push_back(a);
	lines.push_back(d);
	lines.push_back(b);
	lines.push_back(d);
	lines.push_back(c);
	lines.push_back(d);
}

void Object::addPoint(glm::vec3 a) {
	points.push_back(a);
}
