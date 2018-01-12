/*
 * Material.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Material.h"
#include "GLFunctions.h"
#include "GL/glew.h"

Material::Material(GLuint shader, char* name)
{
	this->shader = shader;
	texture = NULL;
	this->name = name;
}

Material::~Material() {
	glDeleteShader(shader);
}

void Material::addTexture(GLuint tex)
{
	texture = tex;
}
