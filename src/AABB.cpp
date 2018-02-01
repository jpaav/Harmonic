#include "AABB.h"



AABB::AABB()
{
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
		0.5, -0.5, -0.5, 1.0,
		0.5,  0.5, -0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,
		0.5, -0.5,  0.5, 1.0,
		0.5,  0.5,  0.5, 1.0,
		-0.5,  0.5,  0.5, 1.0,
	};
	glGenBuffers(1, &aabbBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, aabbBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


AABB::~AABB()
{
	glDeleteBuffers(1, &aabbBuffer);
}

void AABB::update(std::vector<glm::vec3> objVertices)
{
	minx = objVertices[0].x;
	miny = objVertices[0].y;
	minz = objVertices[0].z;
	maxx = objVertices[0].x;
	maxy = objVertices[0].y;
	maxz = objVertices[0].z;
	for (int i = 1; i < objVertices.size(); i++) {
		glm::min(minx, objVertices[i].x);
		glm::min(miny, objVertices[i].y);
		glm::min(minz, objVertices[i].z);
		glm::max(maxx, objVertices[i].x);
		glm::max(maxy, objVertices[i].y);
		glm::max(maxz, objVertices[i].z);
	}
	size = glm::vec3(maxx - minx, maxy - miny, maxz - minz);
	center = glm::vec3((minx + maxx) / 2, (miny + maxy) / 2, (minz + maxz) / 2);
	transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);
}

void AABB::draw(GLuint shader, Transform transform, Camera* cam)
{
	glUseProgram(shader);

	//Generating the MVP matrix
	glm::mat4 LocationMatrix = glm::translate(transform.position);
	glm::mat4 ScalingMatrix = glm::scale(transform.scale);
	//glm::mat4 RoationMatrix =
	glm::mat4 MVPmatrix = cam->dynamicCameraMatrix() * LocationMatrix/* * RotaionMatrix*/ * ScalingMatrix * this->transform;

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc = glGetUniformLocation(shader, "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, aabbBuffer);
	//Add Vertex Position Attribute
	glVertexAttribPointer(
		0,			//Attribute 0. must match shader layout
		3,			//size
		GL_FLOAT,	//type
		GL_FALSE,	//normalized?
		0,			//stride
		(void*)0	//array buffer offset
	);
	//glDrawArrays(GL_TRIANGLES, 0, (int)(aabb.size())); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glDisableVertexAttribArray(0);
}
