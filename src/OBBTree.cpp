//
// Created by jpaavola on 2/7/2018.
//

#include "OBBTree.h"
#include "glm/detail/type_mat.hpp"



OBBTree::OBBTree(std::vector<glm::vec3> *vertices, Transform* transform, bool isAABB) {

	this->transform = transform;

	this->isAABB = isAABB;

	std::vector<Triangle> triangles = loadTriangles(*vertices);
	if(!triangles.empty()){
		//Mean matrix
		glm::vec3 mean = glm::vec3(0,0,0);
		for (auto &triangle : triangles) {
			mean = mean + triangle[0] + triangle[1] + triangle[2];
		}
		center = (1/(3 * (float)triangles.size())) * mean;
		std::cout << "Mean point:\n" << glm::to_string(center) << std::endl;

		if(!isAABB) {
			//Covariance matrix
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					covarianceMatrix[i][j] = 0.0;
					for (auto &vertex : *vertices) {
						covarianceMatrix[i][j] += (vertex[i] - mean[i]) * (vertex[j] - mean[j]);
					}
					covarianceMatrix[i][j] /= vertices->size();
				}
			}

			//Using Eigen
			//MatrixXd centered = mat.rowwise() - mat.colwise().mean();
			//MatrixXd cov = (centered.adjoint() * centered) / double(mat.rows() - 1);
			std::cout << "Covariance Matrix:\n" << glm::to_string(covarianceMatrix) << std::endl;

			//Get Eigenvectors from covarianceMatrix
			Eigen::Matrix3f covarianceMatrix_Eigen;
			covarianceMatrix_Eigen << covarianceMatrix[0][0], covarianceMatrix[0][1], covarianceMatrix[0][2],
					covarianceMatrix[1][0], covarianceMatrix[1][1], covarianceMatrix[1][2],
					covarianceMatrix[2][0], covarianceMatrix[2][1], covarianceMatrix[2][2];
			Eigen::EigenSolver<Eigen::Matrix3f> es(covarianceMatrix_Eigen);
			Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
			std::cout << "Eigenvectors from Covariance Matrix:\n" << es.eigenvectors().format(CleanFmt) << std::endl;
			//Convert to glm::mat3
			eigenVectors = glm::mat3(
					es.eigenvectors().col(0)[0].real(), es.eigenvectors().col(0)[1].real(),
					es.eigenvectors().col(0)[2].real(),
					es.eigenvectors().col(1)[0].real(), es.eigenvectors().col(1)[1].real(),
					es.eigenvectors().col(1)[2].real(),
					es.eigenvectors().col(2)[0].real(), es.eigenvectors().col(2)[1].real(),
					es.eigenvectors().col(2)[2].real()
			);

			std::cout << "Eigenvectors Matrix:\n" << glm::to_string(eigenVectors) << std::endl;

			basis.push_back(glm::normalize(glm::fvec3(glm::column(eigenVectors, glm::length_t(0)))));
			basis.push_back(glm::normalize(glm::fvec3(glm::column(eigenVectors, glm::length_t(1)))));
			basis.push_back(glm::normalize(glm::fvec3(glm::column(eigenVectors, glm::length_t(2)))));
		}
		else {
			basis.emplace_back(1,0,0);
			basis.emplace_back(0,1,0);
			basis.emplace_back(0,0,1);
		}

	} else{
		std::cout << "loadTriangles did not return any Triangles." << std::endl;
	}
	// Get bounds
	findBounds(vertices);


	// Setup vertex buffer

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(glm::vec3), boxVertices.data(), GL_STATIC_DRAW);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

}

OBBTree::~OBBTree() {}

std::vector<Triangle> OBBTree::loadTriangles(std::vector<glm::vec3> vertices) {
	std::vector<Triangle> triangles;
	Triangle temp;
	for(unsigned long long i=0; i<vertices.size()/3; i++) {
		temp.clear();
		temp.push_back(vertices[i]);
		temp.push_back(vertices[i+1]);
		temp.push_back(vertices[i+2]);
		triangles.push_back(temp);
	}
	return triangles;
}

void OBBTree::draw(GLuint shader, Transform objTransform, Camera *camera){
	glUseProgram(shader);

	//Generating the MVP matrix

	glm::mat4 LocationMatrix = glm::translate(objTransform.position);
	glm::mat4 ScalingMatrix = glm::scale(objTransform.scale);
	glm::mat4 RotationMatrix = glm::toMat4(objTransform.rotation);
	//glm::mat4 LocationMatrix = glm::translate();
	//glm::mat4 ScalingMatrix = glm::scale();
	//glm::mat4 RotationMatrix = glm::mat4(eigenVectors);
	glm::mat4 viewProjMatrix = camera->dynamicCameraMatrix();
	glm::mat4 modelMatrix = LocationMatrix * RotationMatrix * ScalingMatrix;

	//Get a handle on the MVP/sampler uniforms
	GLint ModelMatLoc = glGetUniformLocation(shader, "model");
	GLint ViewProjMatLoc = glGetUniformLocation(shader, "viewProj");

	GLint ColorLoc = glGetUniformLocation(shader, "color");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(ViewProjMatLoc, 1, GL_FALSE, glm::value_ptr(viewProjMatrix));

	glUniform3fv(ColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glClear(GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	//Add Vertex Position Attribute
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
			0,			//Attribute 0. must match shader layout
			3,			//size
			GL_FLOAT,	//type
			GL_FALSE,	//normalized?
			0,			//stride
			(void*)0	//array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 *
 * @param basis
 * @param vertices
 * @return void
 */
void OBBTree::findBounds(std::vector<glm::vec3> *vertices) {
	if(vertices->size() < 3){   //Check that the size is compatible
		return;
	}
	//Declaration
	float x_pos, y_pos, z_pos, x_b_pos, y_b_pos, z_b_pos;
	float x_neg, y_neg, z_neg, x_b_neg, y_b_neg, z_b_neg;
	float temp_bx, temp_by, temp_bz;
	//std::cout << "i_hat: " << glm::to_string(i_hat) << "k_hat: " << glm::to_string(k_hat) << "j_hat: " << glm::to_string(j_hat) << std::endl;

	//Initialization
	x_b_pos = std::numeric_limits<float>::min();
	y_b_pos = std::numeric_limits<float>::min();
	z_b_pos = std::numeric_limits<float>::min();
	x_b_neg = std::numeric_limits<float>::max();
	y_b_neg = std::numeric_limits<float>::max();
	z_b_neg = std::numeric_limits<float>::max();
	//Find extrema
	for (auto &vertex : *vertices) {
		temp_bx = glm::dot(basis[0], vertex)/glm::length(basis[0]);
		temp_by = glm::dot(basis[1], vertex)/glm::length(basis[1]);
		temp_bz = glm::dot(basis[2], vertex)/glm::length(basis[2]);
		if(x_b_pos < temp_bx) {
			x_b_pos = temp_bx;
			x_pos = vertex.x;
		}
		if(x_b_neg > temp_bx) {
			x_b_neg = temp_bx;
			x_neg = vertex.x;
		}
		if(y_b_pos < temp_by) {
			y_b_pos = temp_by;
			y_pos = vertex.y;
		}
		if(y_b_neg > temp_by) {
			y_b_neg = temp_by;
			y_neg = vertex.y;
		}
		if(z_b_pos < temp_bz) {
			z_b_pos = temp_bz;
			z_pos = vertex.z;
		}
		if(z_b_neg > temp_bz) {
			z_b_neg = temp_bz;
			z_neg = vertex.z;
		}
	}
	//Format extrema
	extrema.emplace_back(x_neg, y_neg, z_neg);
	extrema.emplace_back(x_neg, y_pos, z_neg);
	extrema.emplace_back(x_neg, y_neg, z_pos);
	extrema.emplace_back(x_pos, y_neg, z_neg);
	extrema.emplace_back(x_pos, y_pos, z_neg);
	extrema.emplace_back(x_pos, y_pos, z_pos);
	extrema.emplace_back(x_pos, y_neg, z_pos);
	extrema.emplace_back(x_neg, y_neg, z_pos);

	//Add vertices
	boxVertices.emplace_back(x_neg, y_neg, z_neg);
	boxVertices.emplace_back(x_neg, y_neg, z_pos);
	boxVertices.emplace_back(x_neg, y_pos, z_pos);

	boxVertices.emplace_back(x_pos, y_pos, z_neg);
	boxVertices.emplace_back(x_neg, y_neg, z_neg);
	boxVertices.emplace_back(x_neg, y_pos, z_neg);

	boxVertices.emplace_back(x_pos, y_neg, z_pos);
	boxVertices.emplace_back(x_neg, y_neg, z_neg);
	boxVertices.emplace_back(x_pos, y_neg, z_neg);

	boxVertices.emplace_back(x_pos, y_pos, z_neg);
	boxVertices.emplace_back(x_pos, y_neg, z_neg);
	boxVertices.emplace_back(x_neg, y_neg, z_neg);

	boxVertices.emplace_back(x_neg, y_neg, z_neg);
	boxVertices.emplace_back(x_neg, y_pos, z_pos);
	boxVertices.emplace_back(x_neg, y_pos, z_neg);

	boxVertices.emplace_back(x_pos, y_neg, z_pos);
	boxVertices.emplace_back(x_neg, y_neg, z_pos);
	boxVertices.emplace_back(x_neg, y_neg, z_neg);

	boxVertices.emplace_back(x_neg, y_pos, z_pos);
	boxVertices.emplace_back(x_neg, y_neg, z_pos);
	boxVertices.emplace_back(x_pos, y_neg, z_pos);

	boxVertices.emplace_back(x_pos, y_pos, z_pos);
	boxVertices.emplace_back(x_pos, y_neg, z_neg);
	boxVertices.emplace_back(x_pos, y_pos, z_neg);

	boxVertices.emplace_back(x_pos, y_neg, z_neg);
	boxVertices.emplace_back(x_pos, y_pos, z_pos);
	boxVertices.emplace_back(x_pos, y_neg, z_pos);

	boxVertices.emplace_back(x_pos, y_pos, z_pos);
	boxVertices.emplace_back(x_pos, y_pos, z_neg);
	boxVertices.emplace_back(x_neg, y_pos, z_neg);

	boxVertices.emplace_back(x_pos, y_pos, z_pos);
	boxVertices.emplace_back(x_neg, y_pos, z_neg);
	boxVertices.emplace_back(x_neg, y_pos, z_pos);

	boxVertices.emplace_back(x_pos, y_pos, z_pos);
	boxVertices.emplace_back(x_neg, y_pos, z_pos);
	boxVertices.emplace_back(x_pos, y_neg, z_pos);

}

glm::vec3 OBBTree::getExtrema(size_t index) {
	return glm::vec3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::vec4(extrema[index], 1.0f));
}

glm::vec3 OBBTree::getCenter() {
	return glm::vec3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::vec4(center, 1.0f));
}

glm::vec3 OBBTree::getTrueAxis(int axisIndex) {
	return glm::vec3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::vec4(glm::column(covarianceMatrix, axisIndex), 1.0f));
}

glm::mat3 OBBTree::getBasis() {
	return glm::mat3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::mat4(eigenVectors));
}

glm::vec3 OBBTree::getMaxes() {
	return glm::vec3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::vec4(extrema[5], 1.0f));
}

glm::vec3 OBBTree::getMins() {
	return glm::vec3(glm::translate(transform->position) * glm::toMat4(transform->rotation) * glm::scale(transform->scale) * glm::vec4(extrema[0], 1.0f));
}
