#pragma once
#include <vector>
//Graphics package
#include "GL/glew.h"
//#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"
#include "UIElement.h"
class Canvas
{
private:
	std::vector<UIElement*> elements;
protected:
	GLuint vbo;
	GLuint shader;
public:
	Canvas(GLuint shader);
	~Canvas();

	void drawAll();
	void addElement(UIElement *element);
	void hideElement(std::string);
};

