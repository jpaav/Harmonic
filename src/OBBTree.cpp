//
// Created by jpaavola on 2/7/2018.
//

#include "OBBTree.h"
#include "glm/detail/type_mat.hpp"



OBBTree::OBBTree(std::vector<glm::vec3> *vertices, Transform* transform) {

	this->transform = transform;

	std::vector<Triangle> triangles = loadTriangles(*vertices);
	if(!triangles.empty()){
		//Mean matrix
		glm::vec3 mean = glm::vec3(0,0,0);
		for (auto &triangle : triangles) {
			mean = mean + triangle[0] + triangle[1] + triangle[2];
		}
		center = (1/(3 * (float)triangles.size())) * mean;
		std::cout << "Mean point:\n" << glm::to_string(center) << std::endl;
		//Covariance matrix
		glm::mat3 covSum = glm::mat3();

		glm::vec3 p, q, r;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				covarianceMatrix[i][j] = 0.0;
				for (int k = 0; k < vertices->size(); k++)
					covarianceMatrix[i][j] += (mean[i] - vertices->at(k)[i]) *
					                          (mean[j] - vertices->at(k)[j]);
				covarianceMatrix[i][j] /= vertices->size() - 1;
			}
		/*for (auto &triangle : triangles) {
			/*covSum +=   (triangle[0]-center)*(triangle[0]-center)*
						(triangle[1]-center)*(triangle[1]-center)*
						(triangle[2]-center)*(triangle[2]-center);*/
			/*covSum +=   p[0]*p[0] + p[1]*p[1] + p[2]*p[2] + p[0]*p[1] + p[0]*p[2] + p[1]*p[2] +
						q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[0]*q[1] + q[0]*q[2] + q[1]*q[2] +
						r[0]*r[0] + r[1]*r[1] + r[2]*r[2] + r[0]*r[1] + r[0]*r[2] + r[1]*r[2];
			covSum += (triangle[0].x-center.x)*(triangle[0].y-center.y)*(triangle[0].z-center.z)
					+ (triangle[1].x-center.x)*(triangle[1].y-center.y)*(triangle[1].z-center.z)
					+ (triangle[2].x-center.x)*(triangle[2].y-center.y)*(triangle[2].z-center.z);
		}*/
		covarianceMatrix = (1/(3 * (float)triangles.size())) * covSum;
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
	GLint MatrixLoc =	glGetUniformLocation(shader, "MVP");

	GLint ColorLoc = glGetUniformLocation(shader, "color");

	//Send MVP to shader in uniform variable
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, glm::value_ptr(MVPmatrix));

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
void OBBTree::findBounds(glm::mat3 basis, std::vector<glm::vec3> *vertices) {
	if(vertices->size() < 3){   //Check that the size is compatible
		return;
	}
	//Declaration
	float x_pos, y_pos, z_pos, x_b_pos, y_b_pos, z_b_pos;
	float x_neg, y_neg, z_neg, x_b_neg, y_b_neg, z_b_neg;
	glm::vec3 temp_b;
	glm::vec3 vertex;
	//Initialization
	vertex = (*vertices)[0];
	temp_b = basis * vertex;
	x_pos = vertex.x;
	y_pos = vertex.y;
	z_pos = vertex.z;
	x_neg = vertex.x;
	y_neg = vertex.y;
	z_neg = vertex.z;
	x_b_pos = temp_b.x;
	y_b_pos = temp_b.y;
	z_b_pos = temp_b.z;
	x_b_neg = temp_b.x;
	y_b_neg = temp_b.y;
	z_b_neg = temp_b.z;
	//Find extrema
	for (int i=1; i < (*vertices).size(); i++) {
		vertex = (*vertices)[i];
		temp_b = basis * vertex;
		if(x_b_pos < temp_b.x) {
			x_b_pos = temp_b.x;
			x_pos = vertex.x;
		}
		if(x_b_neg > temp_b.x) {
			x_b_neg = temp_b.x;
			x_neg = vertex.x;
		}
		if(y_b_pos < temp_b.y) {
			y_b_pos = temp_b.y;
			y_pos = vertex.y;
		}
		if(y_b_neg > temp_b.y) {
			y_b_neg = temp_b.y;
			y_neg = vertex.y;
		}
		if(z_b_pos < temp_b.z) {
			z_b_pos = temp_b.z;
			z_pos = vertex.z;
		}
		if(z_b_neg > temp_b.z) {
			z_b_neg = temp_b.z;
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