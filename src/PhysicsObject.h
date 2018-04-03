/*
 * PhysicsObject.h
 *
 *  Created on: Nov 2, 2017
 *      Author: josephpaavola
 */

#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_

#include "Object.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "AABB.h"
#include "OBBTree.h"
#include "Collision.h"

class PhysicsObject: public Object {
public:
	//Constructors and Destructors
	PhysicsObject(Camera *camera, Material *material, GLuint shader);

	~PhysicsObject() override;
	//Methods
	Collision* updateCollisions(PhysicsObject *otherObject);
	void updateForces(double deltaT, std::vector<glm::vec3> globalForces);
	void applyForce(double deltaT, glm::vec3 force);
	void setMass(float mass) { this->mass = mass; }
	void draw() override;
	void setObjectData(const char* objPath) override ;
	bool isColliding() { return colliding; };
	void getPointsAndLines(std::vector<glm::vec3> *allLines) override{
		float pointSize = 2;
		if(drawCenter) { points.push_back(obbTree->getCenter()); }
		if(drawBBBasis){
			glm::vec3 center = obbTree->getCenter();
			auto basis = obbTree->getBasis();
			//auto basis = obbTree->basis;
			lines.push_back(basis*glm::vec3(center.x, center.y-pointSize, center.z));
			lines.push_back(basis*glm::vec3(center.x, center.y+pointSize, center.z));
			lines.push_back(basis*glm::vec3(center.x-pointSize, center.y, center.z));
			lines.push_back(basis*glm::vec3(center.x+pointSize, center.y, center.z));
			lines.push_back(basis*glm::vec3(center.x, center.y, center.z-pointSize));
			lines.push_back(basis*glm::vec3(center.x, center.y, center.z+pointSize));

		}
		for(auto point : points) {
			lines.emplace_back(point.x, point.y-pointSize, point.z);
			lines.emplace_back(point.x, point.y+pointSize, point.z);
			lines.emplace_back(point.x-pointSize, point.y, point.z);
			lines.emplace_back(point.x+pointSize, point.y, point.z);
			lines.emplace_back(point.x, point.y, point.z-pointSize);
			lines.emplace_back(point.x, point.y, point.z+pointSize);
		}
		allLines->insert(allLines->end(), lines.begin(), lines.end());
		points.clear();
		lines.clear();
	}
	//Instance variables
	bool isPinned;
	OBBTree *obbTree;
	GLuint shader;
	//Static functions
	glm::vec3 static combineForces(std::vector<glm::vec3> forces)
	{
		glm::vec3 forceSum = glm::vec3(0.0f, 0.0f, 0.0f);
		for (auto &force : forces) {
			forceSum.x += force.x;
			forceSum.y += force.y;
			forceSum.z += force.z;
		}
		return forceSum;
	}
	void toggleDrawCenter()  { drawCenter = !drawCenter;  }
	void toggleDrawBBBasis() { drawBBBasis = !drawBBBasis;}
private:
	float mass;
	AABB aabb;
	bool colliding;
	bool drawCenter;
	bool drawBBBasis;
};

#endif /* PHYSICSOBJECT_H_ */
