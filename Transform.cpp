#include "Transform.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Transform::Transform() {
	// Initialize M to identity matrix, and 0 children
	M = glm::mat4(1);
}

Transform::~Transform() {
	// Delete all children
	//for (Node* child: children) {
	//	delete child;
	//}
}

void Transform::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader) {
	// C = model matrix of parent (for world, C = 4x4 identity matrix) aka transformation of parent
	// Do M * C to transform the current geometry/transform based on the parent's transformation
	glm::mat4 matToPass = C * M;
	// std::cout << glm::to_string(M) << std::endl;
	// Loop through all children and call their draw's
	for (Node* child : children) {
		// Pass in transform matrix so that child knows where to be relative to this transform
		child->draw(matToPass, view, projection, viewDir, shader);
	}
}

void Transform::transform(glm::mat4 transformMatrix) {
	// Add a transformation to the transform matrix
	M = transformMatrix * M;
}

void Transform::addChild(Node* child) {
	children.push_back(child);
}

void Transform::update() {

}