#include "TextBox.h"



TextBox::TextBox(std::string name, int fontSize, float xPosition, float yPosition, float xScale, float yScale, float lineSpacing) 
	: UIElement(name, UIElement::UIElementType::TEXT_BOX, xPosition, yPosition, xScale, yScale, lineSpacing)
{
	this->text = "Temp";
	
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return;
	}

	if (FT_New_Face(ft, "C:/Users/jpaavola/Documents/Code/Harmonic/src/fonts/FreeSans.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	g = face->glyph;

	vbo = 0;
	glGenBuffers(1, &vbo);
	


	vao = 0;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenTextures(1, &textureID);
}


TextBox::~TextBox()
{
}

int TextBox::render(GLuint shader)
{
	int num_chars = text.length();
	float tempx = x;
	float tempy = y;
	float tempsx = sx;
	float tempsy = sy;

	for (int i = 0; i<num_chars; i++) {
		if (text[i] == '\n') {
			tempx = x;
			tempy += lineSpacing;
			continue;
		}
		glUseProgram(shader);
		if (FT_Load_Char(face, text[i], FT_LOAD_DEFAULT)) {
			fprintf(stderr, "Could not load char");
			continue;
		}
		if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
			fprintf(stderr, "Could not render glyph");
			continue;
		}
			
		glEnable(GL_TEXTURE_2D);
	

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);
		glUniform1i(glGetUniformLocation(shader, "textureMap"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		//glBindVertexArray(vao);

		float x2 = tempx + g->bitmap_left * tempsx;
		float y2 = -tempy - g->bitmap_top * tempsy;
		float w = g->bitmap.width * tempsx;
		float h = g->bitmap.rows * tempsy;

		GLfloat box[4][4] = {
			{ x2,     -y2    , 0, 0 },
			{ x2 + w, -y2    , 1, 0 },
			{ x2,     -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL
		);
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(0);
		tempx += (g->advance.x / 64) * tempsx;
		tempy += (g->advance.y / 64) * tempsy;
	}
	return 0;
}

void TextBox::setText(std::string text) {
	//Serious memory issues with this line vvv
	this->text.clear();
	this->text.assign(text);
}
