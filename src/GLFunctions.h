/*
 * GLFunctions.h
 *
 *  Created on: Nov 14, 2016
 *      Author: Joseph
 */

#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_

#include "GL/glew.h"
//#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "Color.h"
#include "Camera.h"

void loadTexture(const char*, GLuint*);

void drawVertices(GLfloat[], GLuint, GLuint, int, int);
void drawBox(GLfloat[], GLfloat[], Camera, Color);

std::string readFile(const char *filePath);

GLuint LoadShader(const char *vertex_path,const char *fragment_path, bool hasTransformFeedback);

void drawLine(GLfloat[], GLfloat[], float, Color);

bool loadObj(const char*, std::vector<glm::vec3>*,std::vector<glm::vec2>*,std::vector<glm::vec3>*);

std::vector<glm::vec3> floatArrayToGLMVector(GLfloat *array, size_t length);

#endif /* GLFUNCTIONS_H_ */
