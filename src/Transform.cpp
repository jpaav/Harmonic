#include "Transform.h"



Transform::Transform()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}


Transform::~Transform()
{
}

void Transform::print()
{
	std::cout
		<< "Object Hash: " << typeid(this).hash_code() << std::endl
		<< "Position: (" << position.x << "," << position.y << "," << position.z << ")" << std::endl
		<< "Scale: (" << scale.x << "," << scale.y << "," << scale.z << ")" << std::endl
		<< "Rotation: (" << rotation.x << "," << rotation.y << "," << rotation.z << "," << rotation.w << ")" << std::endl
		<< "Velocity: (" << velocity.x << "," << velocity.y << "," << velocity.z << ")" << std::endl
		<< "Acceleration: (" << acceleration.x << "," << acceleration.y << "," << acceleration.z << ")" << std::endl
		<< std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
		<< std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
}

void Transform::translate(glm::vec3 vector) {
	position += vector;
}
