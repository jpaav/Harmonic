#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "GL/glew.h"
#include "SOIL/SOIL.h"
#include <iostream>

class Texture
{
private:
	GLuint id;
	char* path;
	char* name;
public:
	Texture(char* path, char* name);
	~Texture();

	GLuint* getIdPtr();
};

#endif /* TEXTURE_H_ */
