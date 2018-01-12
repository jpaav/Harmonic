#include "UIElement.h"



UIElement::UIElement(std::string name, UIElement::UIElementType type, float x, float y, float sx, float sy, float lineSpacing)
{
	this->name = name;
	this->type = type;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
	this->lineSpacing = lineSpacing;
	this->visible = true;
}


UIElement::~UIElement()
{
}
