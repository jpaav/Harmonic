#include "PhysicsViewport.h"



PhysicsViewport::PhysicsViewport(GLFWwindow *window, int width, int height, Camera *camera, GLuint shader) : Viewport(window, width, height, camera)
{
	infoShader = shader;
}

PhysicsViewport::PhysicsViewport(GLFWwindow *window, int width, int height, GLuint shader) : Viewport(window, width, height)
{
	infoShader = shader;
}

PhysicsViewport::~PhysicsViewport()
{

}

void PhysicsViewport::addGlobalForce(glm::vec3 force)
{
	globalForces.push_back(force);
}

PhysicsObject* PhysicsViewport::addPhysicsObject(const char* materialName) {
	auto *o = new PhysicsObject(mainCamera, getMaterial(materialName), infoShader);
	objects.push_back(o);
	return o;
}

void PhysicsViewport::updatePhysics()
{
	collisions.clear();
	std::vector<PhysicsObject *> physicsObjects;
	for (auto &object : objects) {
		//Filter master object list by Object type
		if (strcmp(typeid(*object).name(), "PhysicsObjectclass"))
		{
			physicsObjects.push_back((PhysicsObject *) object);
		}
	}
	Collision *temp;
	//TODO: Make sure this doesn't run when there's less than 2 objects
	for (int i = 0; i < physicsObjects.size()-1; ++i) {
		for (int j = i+1; j < physicsObjects.size(); ++j) {
			temp = physicsObjects[j]->updateCollisions(*deltaTime, physicsObjects[i]);
			if(temp != nullptr) {
				collisions.push_back(temp);
			}
		}
	}
	for(auto &physicsObject : physicsObjects) {
		physicsObject->updateForces(*deltaTime, globalForces);
	}

}