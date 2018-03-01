/*
 * Material.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GL/glew.h"


class Material {
private:
	GLuint shader;
	GLuint texture;
	const char* name;
public:
	Material();
	Material(GLuint shader, const char* name);
	virtual ~Material();
	GLuint 	getShader(){ return shader; }
	GLuint 	getTexture(){ return texture; }
	void 	addTexture(GLuint tex);

	const char* getName() const {
		return name;
	}
};

#endif /* MATERIAL_H_ */
