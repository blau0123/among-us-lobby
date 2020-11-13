#include "Transform.h"

#include <fstream>
#include <sstream>
#include <iostream>

Transform::Transform() {
	// Initialize M to identity matrix, and 0 children
	M = glm::mat4(1);
}

void Transform::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, GLuint shader) {
	// C = model matrix of parent (for world, C = 4x4 identity matrix) aka transformation of parent
	// Do M * C to transform the current geometry/transform based on the parent's transformation
	M = C * M;
	// Loop through all children and call their draw's
	for (Node* child : children) {
		// Pass in transform matrix so that child knows where to be relative to this transform
		child->draw(M, view, projection, shader);
	}
}

void Transform::addChild(Node* child) {
	std::cout << "Adding child, so now have " << children.size() << std::endl;
	children.push_back(child);
}

void Transform::update() {

}