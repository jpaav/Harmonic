#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

class UIElement
{
public:
	enum UIElementType
	{
		TEXT_BOX = 0,
	};
	float x;
	float y;
	float sx;
	float sy;
	float lineSpacing;
	bool visible;
	std::string name;
	UIElement(std::string name, UIElementType type, float x, float y, float sx, float sy, float lineSpacing);
	~UIElement();
	virtual int render(GLuint shader) = 0;
private:
	UIElement::UIElementType type;
};

