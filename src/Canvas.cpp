#include "Canvas.h"



Canvas::Canvas(GLuint shader)
{
	this->shader = shader;
	glGenVertexArrays(1, &vbo);
	glBindVertexArray(vbo);
}


Canvas::~Canvas()
{
}

void Canvas::drawAll() {
	for each (UIElement *element in elements)
	{
		if (element->visible) {
			element->render(shader);
		}
	}
}

void Canvas::addElement(UIElement *element) {
	elements.push_back(element);
}

void Canvas::hideElement(std::string name)
{
	for each (UIElement *element in elements) {
		if (element->name == name) {
			element->visible = !element->visible;
		}
	}
}
