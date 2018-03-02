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
	PhysicsObject *o = new PhysicsObject(mainCamera, getMaterial(materialName), infoShader);
	objects.push_back(o);
	return o;
}

void PhysicsViewport::updatePhysics()
{
	std::vector<PhysicsObject *> physicsObjects;
	for (auto &object : objects) {
		//Filter master object list by Object type
		if (strcmp(typeid(*object).name(), "PhysicsObjectclass"))
		{
			physicsObjects.push_back((PhysicsObject *) object);
		}
	}
	int combinations = (int)(physicsObjects.size() * (physicsObjects.size()-1) / 2);
	for (int i = 0; i < combinations; ++i) {
		for (int j = 0; j < combinations - i; ++j) {
			physicsObjects[j]->updateCollisions(*deltaTime, physicsObjects[i]);
		}
	}
	for(auto &physicsObject : physicsObjects) {
		physicsObject->updateForces(*deltaTime, globalForces);
	}

}