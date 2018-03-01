//
// Created by jpaavola on 2/7/2018.
//

#include "OBBTree.h"
#include "glm/detail/type_mat.hpp"



OBBTree::OBBTree(std::vector<glm::vec3> *vertices) {

	std::vector<Triangle> triangles = loadTriangles(*vertices);
	if(!triangles.empty()){
		//Mean matrix
		glm::vec3 mean = glm::vec3(0,0,0);
		for (int i = 0; i < triangles.size(); ++i) {
			mean = mean + triangles[i][0] + triangles[i][1] + triangles[i][2];
		}
		meanMatrix = (1/(3 * (float)triangles.size())) * mean;
		meanMatrix = glm::normalize(meanMatrix);
		std::cout << "Mean Matrix:\n" << glm::to_string(meanMatrix) << std::endl;
		//Covariance matrix
		glm::mat3 covSum = glm::mat3();
		for (int j = 0; j < triangles.size(); ++j) {
			covSum +=   (triangles[j][0]-meanMatrix)*(triangles[j][0]-meanMatrix)*
						(triangles[j][1]-meanMatrix)*(triangles[j][1]-meanMatrix)*
						(triangles[j][2]-meanMatrix)*(triangles[j][2]-meanMatrix);
	}
		covarianceMatrix = (1/(3 * (float)triangles.size())) * covSum;
		std::cout << "Covariance Matrix:\n" << glm::to_string(covarianceMatrix) << std::endl;

		//Get Eigenvectors from covarianceMatrix
		Eigen::Matrix3f covarianceMatrix_Eigen;
		covarianceMatrix_Eigen << covarianceMatrix[0][0], covarianceMatrix[0][1], covarianceMatrix[0][2],
				covarianceMatrix[1][0], covarianceMatrix[1][1], covarianceMatrix[1][2],
				covarianceMatrix[2][0], covarianceMatrix[2][1], covarianceMatrix[2][2];
		Eigen::EigenSolver<Eigen::Matrix3f> es(covarianceMatrix_Eigen);
		Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
		//std::cout << "Eigenvectors from Covariance Matrix:\n" << es.eigenvectors().format(CleanFmt) << std::endl;
		//Assign to a glm::mat2x3
		eigenVectors = glm::mat3(
				es.eigenvectors().col(0)[0].real(), es.eigenvectors().col(0)[1].real(), es.eigenvectors().col(0)[2].real(),
				es.eigenvectors().col(1)[0].real(), es.eigenvectors().col(1)[1].real(), es.eigenvectors().col(1)[2].real(),
				es.eigenvectors().col(2)[0].real(), es.eigenvectors().col(2)[1].real(), es.eigenvectors().col(2)[2].real()
		);

		std::cout << "Eigenvectors Matrix:\n" << glm::to_string(eigenVectors) <<  std::endl;

	} else{
		std::cout << "loadTriangles did not return any Triangles." << std::endl;
	}
	// Get bounds
	findBounds(eigenVectors, vertices);


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
	glm::mat4 MVPmatrix = camera->dynamicCameraMatrix() * LocationMatrix * ScalingMatrix * RotationMatrix; //Use the rotation tutorial to learn change of basis

	//Get a handle on the MVP/sampler uniforms
	GLuint MatrixLoc =	glGetUniformLocation(shader, "MVP");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, &MVPmatrix[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_DEPTH_BUFFER_BIT);

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
void OBBTree::findBounds(glm::mat3 basis, std::vector<glm::vec3> *vertices) {
	float x_pos=0, y_pos=0, z_pos=0, x_b_pos=0, y_b_pos=0, z_b_pos=0;
	float x_neg=0, y_neg=0, z_neg=0, x_b_neg=0, y_b_neg=0, z_b_neg=0;
	glm::vec3 temp_b;
	for (auto vertex : *vertices) {
		temp_b = basis * vertex;
		if(x_b_pos > temp_b.x) {
			x_b_pos = temp_b.x;
			x_pos = vertex.x;
		}
		if(x_b_neg < temp_b.x) {
			x_b_neg = temp_b.x;
			x_neg = vertex.x;
		}
		if(y_b_pos > temp_b.y) {
			y_b_pos = temp_b.y;
			y_pos = vertex.y;
		}
		if(y_b_neg < temp_b.y) {
			y_b_neg = temp_b.y;
			y_neg = vertex.y;
		}
		if(z_b_pos > temp_b.z) {
			z_b_pos = temp_b.z;
			z_pos = vertex.z;
		}
		if(z_b_neg < temp_b.z) {
			z_b_neg = temp_b.z;
			z_neg = vertex.z;
		}
	}
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
