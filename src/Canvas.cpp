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
	for (int i=0; i<elements.size(); i++)
	{
		if (elements[i]->visible) {
			elements[i]->render(shader);
		}
	}
}

void Canvas::addElement(UIElement *element) {
	elements.push_back(element);
}

void Canvas::hideElement(std::string name)
{
	for (int i=0; i<elements.size(); i++)
	{
		if (elements[i]->name == name) {
			elements[i]->visible = !elements[i]->visible;
		}
	}
}
