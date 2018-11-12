#include "AABB.h"



AABB::AABB()
{	glGenBuffers(1, &aabbBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, aabbBuffer);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


AABB::~AABB()
{
	glDeleteBuffers(1, &aabbBuffer);
}

void AABB::update(std::vector<glm::vec3> objVertices, Transform objTransform)
{
	//TODO: optimize
	std::vector<glm::vec4> worldVertices;
	for(int i=0; i<objVertices.size(); i++){
		//worldVertices.push_back(glm::translate(objTransform.position) * glm::scale(objTransform.scale) * glm::vec4(objVertices[i], 0));
	}
	minx = worldVertices[0].x;
	miny = worldVertices[0].y;
	minz = worldVertices[0].z;
	maxx = worldVertices[0].x;
	maxy = worldVertices[0].y;
	maxz = worldVertices[0].z;
	for (int i = 1; i < worldVertices.size(); i++) {
		glm::min(minx, worldVertices[i].x);
		glm::min(miny, worldVertices[i].y);
		glm::min(minz, worldVertices[i].z);
		glm::max(maxx, worldVertices[i].x);
		glm::max(maxy, worldVertices[i].y);
		glm::max(maxz, worldVertices[i].z);
	}
	size = glm::vec3(maxx - minx, maxy - miny, maxz - minz);
	center = glm::vec3((minx + maxx) / 2, (miny + maxy) / 2, (minz + maxz) / 2);
	m_transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);
}

void AABB::draw(GLuint shader, std::vector<glm::vec3> objVertices, Transform objTransform, Camera* cam)
{

	update(objVertices, objTransform);

	glUseProgram(shader);

	//glm::mat4 RoationMatrix =
	glm::mat4 MVPmatrix = cam->dynamicCameraMatrix() * m_transform;

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc =	glGetUniformLocation(shader, "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, aabbBuffer);
	glVertexAttribPointer(
			0,			//Attribute 0. must match shader layout
			3,			//size
			GL_FLOAT,	//type
			GL_FALSE,	//normalized?
			0,			//stride
			(void*)0	//array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);


	//glDrawElements(GL_LINE_LOOP, 4, GL_FLOAT, 0);
	//glDrawElements(GL_LINE_LOOP, 4, GL_FLOAT, (GLvoid*)(4 * sizeof(GLfloat)));
	//glDrawElements(GL_LINES, 8, GL_FLOAT, (GLvoid*)(8 * sizeof(GLfloat)));
}
