
/*
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(std::getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(std::getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
*/

#include "GLFunctions.h"

//Graphics Package
#include "GL/glew.h"
#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"
//Standard Libraries
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include "Color.h"
//GLM libraries
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL/SOIL.h"

void drawVertices(GLfloat vertices[], GLuint vb, GLuint shader, int width, int height)//, Camera cam)
{
	//Run shader
	glUseProgram(shader);

	//Generating the MVP matrix
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::mat4 CameraMatrix = glm::lookAt(
			glm::vec3(4.0f,3.0f,3.0f),
			glm::vec3(0.0f,0.0f,0.0f),
			glm::vec3(0.0f,1.0f,0.0f)
	);
	glm::mat4 ProjectionMatrix = glm::perspective(
			45.0f,
			4.0f/3.0f,
			0.1f,
			10.0f
	);
	glm::mat4 MVPmatrix = ProjectionMatrix * CameraMatrix * ModelMatrix;

	//Get a handle on the MVP uniform
	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPmatrix[0][0]);
	//Set up to draw
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(
		0,			//Attribute 0. must match shader layout
		3,			//size
		GL_FLOAT,	//type
		GL_FALSE,	//normalized?
		0,			//stride
		(void*)0	//array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

std::string readFile(const char *filePath) {
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

/**
 * Loads Vertex Shader from vertex_path and Fragment shader from fragment_path and compiles them, returning the\
 * resulting program id as a GLuint.
 * @param vertex_path
 * @param fragment_path
 * @return
 */
GLuint LoadShader(const char *vertex_path, const char *fragment_path, bool hasTransformFeedback) {
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
    glGetShaderInfoLog(vertShader, logLength, &logLength, vertErrorLog.data());
	if (vertErrorLog.size() > 0) {
		std::cout << vertErrorLog.data() << std::endl;
	}

    // Compile fragment shader
    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> fragErrorLog(logLength);
    glGetShaderInfoLog(fragShader, logLength, &logLength, fragErrorLog.data());
	if (fragErrorLog.size() > 0) {
		std::cout << fragErrorLog.data() << std::endl;
	}
		

    std::cout << "Creating Program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
	if(hasTransformFeedback) {
		//Setup Transform Feedback
		std::cout << "Setting Transform Feedback" << std::endl;
		const char *varyings[] =
			{
				"outPosition"
			};
		glTransformFeedbackVaryings(program, 1, varyings, GL_INTERLEAVED_ATTRIBS);
	}

	//Link
	std::cout << "Linking Program" << std::endl;
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


void drawLine(GLfloat startPoint[], GLfloat endPoint[], float lineWidth=2.5, Color c = Color()) {
	glLineWidth(lineWidth);
	glColor3f(c.r(), c.g(), c.b());
	glBegin(GL_LINES);
	glVertex3f(startPoint[0], startPoint[1], startPoint[2]);
	glVertex3f(endPoint[0], endPoint[1], endPoint[2]);
	glEnd();
}

/*void drawBox(GLfloat start[], GLfloat end[], Camera cam, Color c)
{
	GLfloat vertices[] = {
		    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
		    -1.0f,-1.0f, 1.0f,
		    -1.0f, 1.0f, 1.0f, // triangle 1 : end
		    1.0f, 1.0f,-1.0f, // triangle 2 : begin
		    -1.0f,-1.0f,-1.0f,
		    -1.0f, 1.0f,-1.0f, // triangle 2 : end
		    1.0f,-1.0f, 1.0f,
		    -1.0f,-1.0f,-1.0f,
		    1.0f,-1.0f,-1.0f,
		    1.0f, 1.0f,-1.0f,
		    1.0f,-1.0f,-1.0f,
		    -1.0f,-1.0f,-1.0f,
		    -1.0f,-1.0f,-1.0f,
		    -1.0f, 1.0f, 1.0f,
		    -1.0f, 1.0f,-1.0f,
		    1.0f,-1.0f, 1.0f,
		    -1.0f,-1.0f, 1.0f,
		    -1.0f,-1.0f,-1.0f,
		    -1.0f, 1.0f, 1.0f,
		    -1.0f,-1.0f, 1.0f,
		    1.0f,-1.0f, 1.0f,
		    1.0f, 1.0f, 1.0f,
		    1.0f,-1.0f,-1.0f,
		    1.0f, 1.0f,-1.0f,
		    1.0f,-1.0f,-1.0f,
		    1.0f, 1.0f, 1.0f,
		    1.0f,-1.0f, 1.0f,
		    1.0f, 1.0f, 1.0f,
		    1.0f, 1.0f,-1.0f,
		    -1.0f, 1.0f,-1.0f,
		    1.0f, 1.0f, 1.0f,
		    -1.0f, 1.0f,-1.0f,
		    -1.0f, 1.0f, 1.0f,
		    1.0f, 1.0f, 1.0f,
		    -1.0f, 1.0f, 1.0f,
		    1.0f,-1.0f, 1.0f
		};
		drawVertices(vertices, vb, shader, cam.getWidth(), cam.getHeight());
	/*GLfloat vertices = {
			start[0],	start[1],	start[2],//tri 1
			end[0],		start[1],	start[2],
			start[0],	end[1],		start[2],
			end[0],		end[1],		start[2],//tri2
			start[0],	end[1],		start[2],
			end[0],		start[1],	start[2],
			end[0], 	start[1], 	start[2],//tri3
			start[0],	start[1],	start[2],
			start[0],	start[1],	end[2],
			end[0],		start[1],	start[2],//tri4
			end[0],		start[1],	end[2],
			start[0],	start[1],	end[2],
			start[0],	start[1],	start[2],//tri5
			start[0],	start[1],	end[2],
			start[0],	end[1],		start[2],
			start[0],	end[1],		start[2],//tri6
			start[0],	end[1],		end[2],
			start[0],	start[1],	end[2],
			end[0],		start[1],	start[2],//tri7
			end[0],		end[1],		start[2],
			end[0],		start[1],	end[2],
			end[0],		end[1],		start[2],//tri8
			end[0],		end[1],		end[2],
			end[0],		start[1],	end[2],
			start[0],	start[1],	end[2],//tri9
			start[0],	end[1],		end[2],
			end[0],		start[1],	end[2],
			start[0],	end[1],		end[2],//tri10
			end[0],		end[1],		end[2],
			end[0],		start[1],	end[2],
			start[0],	end[1],		start[2],//tri11
			end[0],		end[1],		start[2],
			end[0],		end[1],		end[2],
			start[0],	end[1],		start[2],//tri12
			start[0],	end[1],		end[2],
			end[0],		end[1],		end[2]
	};


}*/

/*GLuint loadTexture(const char* path)
{
	int width, height, n;
	GLuint textureID;
	//Get Data
	unsigned char *data = stbi_load(path, &width, &height, &n, 0);
	if(data==NULL)
	{
		printf("Image file corrupted or an allocator failed.");
		stbi_image_free(data);
		return 0;
	}

	//Make, bind and send texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return textureID;
}*/

void loadTexture(const char* path, GLuint* texture)
{
	*texture = SOIL_load_OGL_texture(path,
		 SOIL_LOAD_AUTO,
		 SOIL_CREATE_NEW_ID,
		 SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);
	//TODO: This was *texture=NULL
	if(!*texture){
		printf("[Texture loader] \"%s\" failed to load!\n", path);
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

bool loadObj(const char* path, std::vector<glm::vec3> *out_vertices, std::vector<glm::vec2> *out_uvs,
             std::vector<glm::vec3> *out_normals){
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if(file==NULL){
		printf("Impossible to open file!\n");
		return false;
	}
	while(1){
		char lineHeader[1024];
		//read first word
		int res = fscanf(file, "%s", lineHeader);	//fnscanf?
		//Stops at end of file
		if(res==EOF){
			break;
		}
		//Parse vertices
		if(strcmp(lineHeader, "v")==0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}//Parse UVs
		else if(strcmp(lineHeader, "vt")==0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}//Parse normals
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}//Parse faces
		else if(strcmp(lineHeader, "f")==0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			                     &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			                     &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			                     &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if(matches!=9){
				printf("Try other options, this parser is too basic for that kind of file!\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		//Process the object data
		for(unsigned int i=0; i<vertexIndices.size(); i++){
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex-1];	//OBJ indexing starts at 1
			out_vertices->push_back(vertex);
		}
		for(unsigned int i=0; i<uvIndices.size(); i++){
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex-1];	//OBJ indexing starts at 1
			out_uvs->push_back(uv);
		}
		for(unsigned int i=0; i<normalIndices.size(); i++){
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex-1];	//OBJ indexing starts at 1
			out_normals->push_back(normal);
		}
	}
	return true;
}

std::vector<glm::vec3> floatArrayToGLMVector(GLfloat *array, size_t length) {
	std::vector<glm::vec3> vector;
	//TODO: fix errors with this check vvv
	//if(length % 3 != 0){ return NULL; }
	for (int i = 0; i < length; i+=3) {
		vector.emplace_back(array[i], array[i+1], array[i+2]);
	}
	return vector;
}
