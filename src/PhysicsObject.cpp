/*
 * PhysicsObject.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: josephpaavola
 */

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Camera *camera, Material *material) : Object(camera, material){
	
	mass = 1.0;
	isPinned = false;

}

PhysicsObject::~PhysicsObject() {
	mass = 1.0;
	isPinned = false;
}

void PhysicsObject::update(double deltaT, std::vector<glm::vec3> globalForces) {
	//Stop update if object doesn't move
	//This may be changed later when collisions are added
	if (isPinned) { return; }
	//Combine global and local forces
	std::vector<glm::vec3> forces;
	forces.push_back(combineForces(globalForces));
	forces.push_back(combineForces(transform.localForces));
	//Apply forces
	applyForce(deltaT, combineForces(forces));
}

void PhysicsObject::applyForce(double deltaT, glm::vec3 force)
{
	double p_x, p_y, p_z, v_x, v_y, v_z;

	transform.acceleration.x = force.x / mass;
	transform.acceleration.y = force.y / mass;
	transform.acceleration.z = force.z / mass;

	v_x = transform.velocity.x + (deltaT * transform.acceleration.x);
	v_y = transform.velocity.y + (deltaT * transform.acceleration.y);
	v_z = transform.velocity.z + (deltaT * transform.acceleration.z);

	p_x = transform.position.x + (transform.velocity.x*deltaT) + (0.5*transform.acceleration.x*deltaT*deltaT);
	p_y = transform.position.y + (transform.velocity.y*deltaT) + (0.5*transform.acceleration.y*deltaT*deltaT);
	p_z = transform.position.z + (transform.velocity.z*deltaT) + (0.5*transform.acceleration.z*deltaT*deltaT);

	transform.velocity = glm::vec3(v_x, v_y, v_z);
	transform.position = glm::vec3(p_x, p_y, p_z);
}

std::vector<glm::vec3> PhysicsObject::updateAABB()
{
	float minx = vertices[0].x;
	float miny = vertices[0].y;
	float minz = vertices[0].z;
	float maxx = vertices[0].x;
	float maxy = vertices[0].y;
	float maxz = vertices[0].z;
	for (int i = 1; i < vertices.size(); i++) {
		glm::min(minx, vertices[i].x);
		glm::min(miny, vertices[i].y);
		glm::min(minz, vertices[i].z);
		glm::max(maxx, vertices[i].x);
		glm::max(maxy, vertices[i].y);
		glm::max(maxz, vertices[i].z);
	}
	std::vector<glm::vec3> aabb;
	aabb.push_back(glm::vec3(minx, miny, minz));
	aabb.push_back(glm::vec3(maxx, miny, minz));
	aabb.push_back(glm::vec3(minx, maxy, minz));
	aabb.push_back(glm::vec3(minx, miny, maxz));
	aabb.push_back(glm::vec3(maxx, maxy, minz));
	aabb.push_back(glm::vec3(minx, maxy, maxz));
	aabb.push_back(glm::vec3(maxx, miny, maxz));
	aabb.push_back(glm::vec3(maxx, maxy, maxz));
	this->aabb = aabb;
	// I may need to get rid of the local aabb
	glBindBuffer(GL_ARRAY_BUFFER, aabbBuffer);
	glBufferData(GL_ARRAY_BUFFER, aabb.size() * sizeof(glm::vec3), aabb.data(), GL_STATIC_DRAW);
	return aabb;
}

void PhysicsObject::draw() {
	std::cout << "In PhysicsObject::draw" << std::endl;
	Object::draw();

	// Draw aabb

	glUseProgram(m_mat->getShader());

	//Generating the MVP matrix
	glm::mat4 LocationMatrix = glm::translate(transform.position);
	glm::mat4 ScalingMatrix = glm::scale(transform.scale);
	//glm::mat4 RoationMatrix =
	glm::mat4 MVPmatrix = m_cam->dynamicCameraMatrix() * LocationMatrix/* * RotaionMatrix*/ * ScalingMatrix;

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc = glGetUniformLocation(m_mat->getShader(), "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

	glDrawArrays(GL_LINES, 0, 12); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}


