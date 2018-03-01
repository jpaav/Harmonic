/*
 * Viewport.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: Joseph
 */
#include "Viewport.h"
Viewport::Viewport(GLFWwindow *window, int width, int height, Camera *camera){
	this->height = height;
	this->width = width;
	this->window = window;
	//Make Vertex Array Object to send all vertice data to the gpu at once
	glGenVertexArrays(1, &vertexArrayObj);
	glBindVertexArray(vertexArrayObj);
	this->mainCamera = camera;
	deltaTime = new double(-1.0);
	this->mainCamera->setDeltaTimePtr(this->deltaTime);
}
Viewport::Viewport(GLFWwindow *window, int width, int height){
	this->height = height;
	this->width = width;
	this->window = window;
	//Make Vertex Array Object to send all vertice data to the gpu at once
	glGenVertexArrays(1, &vertexArrayObj);
	glBindVertexArray(vertexArrayObj);	this->mainCamera = new Camera(window);
	deltaTime = new double(-1.0f);
	this->mainCamera->setDeltaTimePtr(this->deltaTime);
}

GLuint Viewport::loadShader(const char *vertex_path, const char *fragment_path){
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertErrorLog(logLength);
	glGetShaderInfoLog(vertShader, logLength, &logLength, &vertErrorLog[0]);
	std::cout << &vertErrorLog[0] << std::endl;

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragErrorLog(logLength);
	glGetShaderInfoLog(fragShader, logLength, &logLength, &fragErrorLog[0]);
	std::cout << &fragErrorLog[0] << std::endl;

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError( (logLength > 1) ? logLength : 1 );
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

std::string Viewport::readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

Texture* Viewport::addTexture(std::string path, std::string name) {
	Texture *texture = new Texture(path, name);
	textures.push_back(texture);
	return texture;
}

Material* Viewport::addMaterial(GLuint shader, const char* name){
	Material *material = new Material(shader, name);
	materials.push_back(material);
	return material;
}

Material* Viewport::getMaterial(const char* name){
	for(int i=0; i < materials.size(); i++){
		if(strcmp(materials[i]->getName(), name)==0){
			return materials[i];
		}
	}
	return NULL;
}

Object* Viewport::addObject(int materialIndex){
	Object *o = new Object(mainCamera, materials[materialIndex]);
	objects.push_back(o);
	return o;
}

Object* Viewport::addObject(const char* materialName){
	Object *o = new Object(mainCamera, getMaterial(materialName));
	objects.push_back(o);
	return o;
}

void Viewport::updateCameraPos(){
	mainCamera->updatePos();
}

void Viewport::drawAll(){
	for(int i=0; i < objects.size(); i++){
		objects[i]->draw();
	}
}

void Viewport::drawAllEdges(){
	for(int i=0; i< objects.size(); i++){
		objects[i]->drawEdges();
	}
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


