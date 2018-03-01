/*
 * Driver.cpp
 *
 *  Created on: Nov 13, 2016
 *      Author: Joseph
 */

//Standard functions
#include <iostream>
#include <sstream>
#include "stdio.h"
//My includes
#include "Vector3.h"
#include "Object.h"
#include "Material.h"
#include "GLFunctions.h"
#include "Camera.h"
#include "Viewport.h"
#include "PhysicsViewport.h"
#include "PhysicsObject.h"
#include "TextBox.h"
#include "Canvas.h"
//Graphics package
#include "GL/glew.h"
//#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "OBBTree.h"

//Callbacks
void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	Viewport *v = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	Canvas *c = v->getCanvas();
	Camera *cam = v->getMainCamera();
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, 1);
	}
	else if (key == GLFW_KEY_V) {
		if (cam->mode == STATIC_MODE) {
			cam->mode = TURNTABLE_MODE;
		}
		else if (cam->mode == TURNTABLE_MODE) {
			cam->mode = INPUT_MODE;
		}
		else if (cam->mode == INPUT_MODE) {
			cam->mode = STATIC_MODE;
		}
	}
	else if (key == GLFW_KEY_I && action == 0) {
		c->hideElement("Info");
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Viewport *v = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	Camera *cam = v->getMainCamera();
	cam->fov -= yoffset/10;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Viewport *v = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	Camera *cam = v->getMainCamera();
	if (cam->mode == INPUT_MODE) {
		cam->xpos = xpos;
		cam->ypos = ypos;
	}
	//std::cout << "MOUSE POS: " << xpos << "," << ypos << std::endl;
}


//Main function
int main(int argc, char **argv) {
	glfwSetErrorCallback(error_callback);
	GLFWwindow *window;
	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(800, 600, "Harmonic", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	glfwSetCursorPosCallback(window, mouse_callback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Get window width and height
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//Load shaders
	GLuint shader = LoadShader("C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/simple_v.glsl",
			"C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/simple_f.glsl");
	GLuint diffuseShader = LoadShader("C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/VertexShader.glsl",
		"C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/FragmentShader.glsl");
	GLuint textShader = LoadShader("C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/text_v.glsl",
		"C:/Users/jpaavola/Documents/Code/Harmonic/src/shaders/text_f.glsl");


	//Make a new Viewport object
	PhysicsViewport *vp = new PhysicsViewport(window, width, height, shader);
	vp->addGlobalForce(glm::vec3(0.0f, -9.8f, 0.0f));

	//Binds the Viewport to the window for use during key callbacks
	glfwSetWindowUserPointer(window, vp);


	/*Test Ico//
	vp->addMaterial(shader, "testMat");
	
	Object *box = vp->addObject("testMat");
	box->setObjectData("C:/Users/jpaavola/Documents/Code/Harmonic/Debug/objs/IcoSphere2.obj");
	box->setScale(2.0f, 2.0f, 2.0f);
	//End Test Ico*/
	
	//Random test//
	Color c(1.0f, 1.0f, 1.0f);
	//End Random Test//


	//Test Can//
	const char *canMatName = "canMat";
	Material* canMat = vp->addMaterial(diffuseShader, canMatName);
	Texture* canDiffuse = vp->addTexture("C:/Users/jpaavola/Documents/Code/Harmonic/src/textures/can_Diffuse.png", "canDiffuse");
	canMat->addTexture(*canDiffuse->getIdPtr());
	PhysicsObject* can = vp->addPhysicsObject(canMatName);
	can->setObjectData("C:/Users/jpaavola/Documents/Code/Harmonic/src/objs/can/tinCan.obj");
	can->setLocation(0, 0, 0);
	can->setRotation(0,0,0);
	can->isPinned = true;
	//End Test Can//

	/*Ground plane//
	const char *defaultMatName = "default";
	Material* defaultMat = vp->addMaterial(shader, defaultMatName);
	Object* groundPlane = vp->addObject(defaultMatName);
	groundPlane->setObjectData("C:/Users/jpaavola/Documents/Code/Harmonic/src/objs/Plane.obj");
	groundPlane->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
	//End Ground Plane*/

	//Test OBBTree//
	can->obbTree;
	//End Test OBBTree//

	//Text Setup//
	Canvas *UI = new Canvas(textShader);
	vp->setCanvas(UI);
	float sx = 2.0 / width;
	float sy = 2.0 / height;
	TextBox *infoTB = new TextBox("Info", 16, -1 + 8 * sx, 1-15*sy, sx, sy, -1+275*sy);
	UI->addElement(infoTB);
	std::string fpsString;
	std::string infoCompString;

	std::string vendor;
	vendor.append("Vendor: ").append((char *)glGetString(GL_VENDOR)).append("\n");
	std::string renderer;
	renderer.append("Renderer: ").append((char *)glGetString(GL_RENDERER)).append("\n");
	std::string version;
	version.append("Version: ").append((char *)glGetString(GL_VERSION)).append("\n");
	std::string extensions;
	extensions.append("Extensions: ").append((char *)glGetString(GL_EXTENSIONS)).append("\n");

	//End Text Setup//

	//Blending for text
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	float currentFrame, lastFrame;		//For framerate and deltaTime
	float lastUpdate;					//For updating the simulation

	lastFrame  = glfwGetTime();	//Should this be closer to the currentFrame call?
	lastUpdate = glfwGetTime();
	bool canUpdate = false;
	while (!glfwWindowShouldClose(window)) {
		//Get Time
		currentFrame = glfwGetTime();
		vp->setDeltaTime(currentFrame - lastFrame);
		//Clear Strings
		fpsString.clear();
		infoCompString.clear();
		//Text rendering
		fpsString.append(std::to_string(currentFrame - lastFrame)).append("ms\n");
		
		//Combine strings
		infoCompString.append(vendor).append(version).append(renderer).append(fpsString);

		infoTB->setText(infoCompString);
		//delete (void*)&infoComp;
		lastFrame = currentFrame;
		//Render//

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enables textures (Should it be outside main loop?)
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		//Depth
		glDepthFunc(GL_LESS);
		//Define vieport
		glViewport(0, 0, width, height);
		//Update Camera//
		vp->updateCameraPos();
		//Update Physics
		vp->updatePhysics();
		//Draw All objects//
		vp->drawAll();
		//Draw Canvas//
		UI->drawAll();

		//Update render view by swapping front and back buffers
		glfwSwapBuffers(window);
		//Poll for and process events
		glfwPollEvents();
	}
	//Cleanup
	//glDeleteProgram(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


