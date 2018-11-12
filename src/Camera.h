/*
 * Camera.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Joseph
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#define TURNTABLE_MODE 1
#define INPUT_MODE 2
#define STATIC_MODE 3
#define PERSP_TYPE 1
#define ORTHO_TYPE 2

class Camera {
private:
	GLFWwindow *win;
	glm::vec3 position;
	glm::vec3 focusPoint;
	float nClip;
	float fClip;
	int width;
	int height;
	double *deltaTime;
	float verticalAngle;
	float horizontalAngle;
	float speed;
	float mouseSpeed;
public:
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;
	int mode;
	int type;
	float fov;
	/**
	* Mouse position in screen space
	*/
	double xpos, ypos;
	/**
	 * Last mouse position, used to compare with current and get delta
	 */
	double lastx, lasty;

	Camera();

	explicit Camera(GLFWwindow* window);
	virtual ~Camera();

	void updatePos();
	glm::mat4 cameraMatrix();
	glm::mat4 dynamicCameraMatrix();
	//glm::mat4 tableTurn();

	void setPos(glm::vec3 p){position = p;}
	glm::vec3 getPos(){return position;}
	void setDeltaTimePtr(double* dt){deltaTime = dt;}

	int getWidth(){return width;}
	int getHeight(){return height;}
	void moveWest(float increment);
	void moveEast(float increment);
	void moveNorth(float increment);
	void moveSouth(float increment);
	void moveUp(float increment);
	void moveDown(float increment);
	void rotateLeft(float increment);
	void rotateRight(float increment);
	/**
	 * Increases speed without allowing it below the threshold, 0.05 in this case.
	 * @param increment the increment, positive or negative, that speed will be changed by
	 */
	void changeSpeed(float increment){ if(speed+increment > 0.05) {speed+=increment; } }
	void rotateViewMode();
};

#endif /* CAMERA_H_ */
