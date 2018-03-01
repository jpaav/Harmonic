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

	for (int i=0; i<objects.size(); i++)
	{
		
		if (strcmp(typeid(*objects[i]).name(), "PhysicsObjectclass"))
		{
			((PhysicsObject *)objects[i])->update(*deltaTime, globalForces);
		}
		
	}
}