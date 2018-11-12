/*
 * Material.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GL/glew.h"
#include "Color.h"


class Material {
private:
	GLuint shader;
	GLuint texture;
	Color color;
	GLfloat shininess;
	const char* name;
public:
	Material(GLuint shader, const char* name);
	virtual ~Material();
	GLuint 	getShader(){ return shader; }
	GLuint 	getTexture(){ return texture; }
	void 	addTexture(GLuint tex);

	const char* getName() const {
		return name;
	}

	const Color &getColor() const {
		return color;
	}

	void setColor(const Color &color) {
		Material::color = color;
	}

	GLfloat getShininess() const {
		return shininess;
	}

	GLfloat * getShininessPtr() {
		return &shininess;
	}

	void setShininess(GLfloat shininess) {
		Material::shininess = shininess;
	}
};

#endif /* MATERIAL_H_ */
