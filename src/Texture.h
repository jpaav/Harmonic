#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "GL/glew.h"
#include "SOIL/SOIL.h"
#include <iostream>

class Texture
{
private:
	GLuint id;
	std::string path;
	std::string name;
public:
	Texture(std::string path, std::string name);
	~Texture();

	GLuint* getIdPtr();
};

#endif /* TEXTURE_H_ */
