/*
 * Viewport.h
 *
 *  Created on: Dec 13, 2016
 *      Author: Joseph
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_
#include "GL/glew.h"
#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "PhysicsObject.h"
#include "Texture.h"
#include <vector>
//For loading shaders
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "Color.h"
#include "Canvas.h"
#include "SOIL/SOIL.h"


class Viewport {
protected:
	GLuint vertexArrayObj;
	Camera *mainCamera;
	std::vector<Material*> materials;
	std::vector<Texture*> textures;
	GLFWwindow *window;
	Canvas *canvas;
	int height;
	int width;
	double *deltaTime;
	std::vector<Object*> objects;
	std::vector<glm::vec3> lines;
	GLuint lineVBO;
	GLuint infoShader;

public:
	Viewport(GLFWwindow *window, int width, int height, Camera *camera, GLuint shader);
	Viewport(GLFWwindow *window, int width, int height, GLuint shader);

	virtual ~Viewport();
	//Setup
	Material* addMaterial(GLuint shader, const char* name);
	Material* getMaterial(const char* name);
	Texture* addTexture(std::string path, std::string name);
	Object* addObject(int materialIndex);
	Object* addObject(const char* materialName);
	Object* getObject(const char* objectName);
	//Running
	void updateCameraPos();
	void drawAll();
	void drawAllEdges();
	double* getDeltaTimePtr() const {
		return deltaTime;
	}

	void setDeltaTime(double deltaTime) {
		*(this->deltaTime) = deltaTime;
	}

	Camera* getMainCamera() const {
		return mainCamera;
	}
	void setCanvas(Canvas* canvas) {
		this->canvas = canvas;
	}
	Canvas* getCanvas() {
		return canvas;
	}

	void drawPointsAndLines();
};



#endif /* VIEWPORT_H_ */
