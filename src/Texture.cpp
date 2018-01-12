#include "Texture.h"



Texture::Texture(char* path, char* name)
{
	this->id = 0;
	this->path = path;
	this->name = name;
	id = SOIL_load_OGL_texture(path,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
	);
	if (id == NULL) {
		std::cout << "[Texture loader] \"" << path << "\" failed to load!\n";
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	/*GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned char* data =
	SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	printf("SOIL error: %s", SOIL_last_result());
	std::cout << !data << "\n";
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	return textureID;*/
}


Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

GLuint* Texture::getIdPtr() {
	return &id;
}