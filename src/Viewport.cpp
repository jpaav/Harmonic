/*
 * Viewport.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: Joseph
 */
#include "Viewport.h"

#include <utility>

Viewport::Viewport(GLFWwindow *window, int width, int height, Camera *camera, GLuint shader){
	this->height = height;
	this->width = width;
	this->window = window;
	//Make Vertex Array Object to send all vertice data to the gpu at once
	glGenVertexArrays(1, &vertexArrayObj);
	glBindVertexArray(vertexArrayObj);
	this->mainCamera = camera;
	deltaTime = new double(-1.0);
	this->mainCamera->setDeltaTimePtr(this->deltaTime);
	infoShader = shader;
	// Generate vertex buffer
	glGenBuffers(1, &lineVBO);
}
Viewport::Viewport(GLFWwindow *window, int width, int height, GLuint shader){
	this->height = height;
	this->width = width;
	this->window = window;
	//Make Vertex Array Object to send all vertice data to the gpu at once
	glGenVertexArrays(1, &vertexArrayObj);
	glBindVertexArray(vertexArrayObj);	this->mainCamera = new Camera(window);
	deltaTime = new double(-1.0f);
	this->mainCamera->setDeltaTimePtr(this->deltaTime);
	infoShader = shader;
	// Generate vertex buffer
	glGenBuffers(1, &lineVBO);
}

Viewport::~Viewport(){
	delete deltaTime;
	delete mainCamera;
	/*Still fix this to only call glDeleteTextures() once
	for (int i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, textures[i]->getIdPtr());
	}*/

	for(auto &it:materials) delete it; materials.clear();
	for(auto &it:objects) delete it; objects.clear();
	for(auto &it:textures) delete it; textures.clear();
	glDeleteVertexArrays(1, &vertexArrayObj);
}


Texture* Viewport::addTexture(std::string path, std::string name) {
	Texture *texture = new Texture(std::move(path), name);
	textures.push_back(texture);
	return texture;
}

Material* Viewport::addMaterial(GLuint shader, const char* name){
	auto *material = new Material(shader, name);
	materials.push_back(material);
	return material;
}

Material* Viewport::getMaterial(const char* name){
	for (auto &material : materials) {
		if(strcmp(material->getName(), name)==0){
			return material;
		}
	}
	return nullptr;
}

Object* Viewport::addObject(int materialIndex){
	auto *o = new Object(mainCamera, materials[materialIndex]);
	objects.push_back(o);
	return o;
}

Object* Viewport::addObject(const char* materialName){
	auto *o = new Object(mainCamera, getMaterial(materialName));
	objects.push_back(o);
	return o;
}

void Viewport::updateCameraPos(){
	mainCamera->updatePos();
}

void Viewport::drawAll(){
	//Draw all the objects
	for (auto &object : objects) {
		object->draw();
	}
	drawPointsAndLines();
}

void Viewport::drawAllEdges(){
	for(auto &object : objects){
		object->drawEdges();
	}
}

void Viewport::drawPointsAndLines() {
	glUseProgram(infoShader);

	//Generate vertices
	for(auto &object : objects) {
		object->getPointsAndLines(&lines);
	}
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_STATIC_DRAW);

	//Generating the MVP matrix
	glm::mat4 MVPmatrix = mainCamera->dynamicCameraMatrix();

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc =	glGetUniformLocation(infoShader, "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glClear(GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glVertexAttribPointer(
			0,			//Attribute 0. must match shader layout
			3,			//size
			GL_FLOAT,	//type
			GL_FALSE,	//normalized?
			0,			//stride
			(void*)0	//array buffer offset
	);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));
	glDisableVertexAttribArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	lines.clear();
}

Object *Viewport::getObject(const char *objectName) {
	for(auto object : objects) {
		if(object->getName() == objectName) {
			return object;
		}
	}
}

