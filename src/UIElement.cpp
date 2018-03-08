#include "UIElement.h"

#include <utility>


UIElement::UIElement(std::string name, UIElement::UIElementType type, float x, float y, float sx, float sy, float lineSpacing)
{
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);

	this->name = std::move(name);
	this->type = type;
	this->x = x + 8 * sx;
	this->y = y - 15 * sy;
	this->sx = sx;
	this->sy = sy;
	this->lineSpacing = lineSpacing;
	this->visible = true;
}


UIElement::~UIElement()
{
}
