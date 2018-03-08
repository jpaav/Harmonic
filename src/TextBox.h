#pragma once
//FreeType
#define FT2_BUILD_LIBRARY
#include "ft2build.h"
#include FT_FREETYPE_H
//Graphics package
#include "GL/glew.h"
//#define GLFW_INCLUDE_GL_3
#include <string>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "UIElement.h"


class TextBox : public UIElement
{
private:
	std::string text;
	FT_Library ft;
	FT_Face face;
	FT_GlyphSlot g;
	GLuint vbo;
	GLuint vao;
	GLuint textureID;

public:
	TextBox(std::string name, int fontSize, float xPosition, float yPosition, float xScale, float yScale, float lineSpacing);
	~TextBox() override;
	int render(GLuint shader) override;
	void setText(std::string text);
};

