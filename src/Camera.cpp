/*
 * Camera.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#include "Camera.h"
//OpenGL libraries
#include "GLFW/glfw3.h"
//GLM libraries
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(){
	mode = STATIC_MODE;
	type = PERSP_TYPE;
}
Camera::Camera(GLFWwindow *window) {
	win = window;
	deltaTime = NULL;
	position = glm::vec3(4.0f, 3.0f, 3.0f);
	focusPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	// horizontal angle : toward -Z
	horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	verticalAngle = 0.0f;
	//mouse speed
	mouseSpeed = 0.005f;
	//playback speed
	speed = 3.0f;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	forward = glm::vec3(0.0f,0.0f,-1.0f);
	fov = 45.0f;
	nClip = 0.1f;
	fClip = 100.0f;
	glfwGetFramebufferSize(win, &width, &height);
	mode = STATIC_MODE;
	type = PERSP_TYPE;
	glfwGetCursorPos(window, &xpos, &ypos);
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}
void Camera::updatePos(){
	if(mode==TURNTABLE_MODE || mode==STATIC_MODE)
	{
		return;
	}
	float dt = *deltaTime;
	glfwGetCursorPos(win, &xpos, &ypos);

	horizontalAngle += mouseSpeed * dt * float(xpos );
	verticalAngle   += mouseSpeed * dt * float(ypos );
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	glm::vec3 newRight = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	glm::vec3 newUp = glm::cross( newRight, direction );

	// Move forward
	if (glfwGetKey( win, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * dt * speed;
	}
	// Move backward
	if (glfwGetKey( win, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * dt * speed;
	}
	// Strafe right
	if (glfwGetKey( win, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += newRight * dt * speed;
	}
	// Strafe left
	if (glfwGetKey( win, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= newRight * dt * speed;
	}
	forward = direction;
	up = newUp;
	right = newRight;
}
glm::mat4 Camera::dynamicCameraMatrix()
{
	if(mode==TURNTABLE_MODE){	//Over-rides camera position and returns and turntable animation
		GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view, projection;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		projection = glm::perspective(fov, (float)width/(float)height, nClip, fClip);
		return projection*view;
	} else if(mode==STATIC_MODE){
		return cameraMatrix();
	} else if(mode==INPUT_MODE){
		return glm::perspective(fov, (float)width/(float)height, nClip, fClip) * glm::lookAt(position,position+forward,up);
	} else{
		return cameraMatrix();
	}
}
glm::mat4 Camera::cameraMatrix()
{
	return glm::perspective(fov, (float)width/(float)height, nClip, fClip) * glm::lookAt(position,glm::vec3(0.0f, 0.0f, 0.0f),up);
}
void Camera::moveWest(float increment){
	position.x += increment;
}
void Camera::moveEast(float increment){
	position.x -= increment;
}
void Camera::moveNorth(float increment){
	position.z += increment;
}
void Camera::moveSouth(float increment){
	position.z -= increment;
}
void Camera::moveUp(float increment){
	position.y += increment;
}
void Camera::moveDown(float increment){
	position.y -= increment;
}
void Camera::rotateLeft(float increment){
	glm::vec4 focusVec(position-focusPoint, 0.0f);
	glm::normalize(focusVec);
	focusVec.z = 0.0f;
	glm::mat4 yawAxisMat = glm::rotate(5.0f, up);
	glm::mat4 pitchAxisMat = glm::rotate(5.0f, right);
	focusVec = yawAxisMat * pitchAxisMat * focusVec;
	focusVec += glm::vec4(focusPoint, 0.0f);
	glm::vec3 newPos(focusVec);
	position = newPos;
	glm::lookAt(position,focusPoint,up);
}
void Camera::rotateRight(float increment){
	glm::vec4 focusVec(position-focusPoint, 0.0f);
	focusVec.z = 0.0f;
	glm::mat4 yawAxisMat = glm::rotate(-5.0f, up);
	glm::mat4 pitchAxisMat = glm::rotate(-5.0f, right);
	focusVec = yawAxisMat * pitchAxisMat * focusVec;
	focusVec += glm::vec4(focusPoint, 0.0f);
	glm::vec3 newPos(focusVec);
	position = newPos;
	glm::lookAt(position,focusPoint,up);
}
