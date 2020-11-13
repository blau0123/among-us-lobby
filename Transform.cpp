#include "Transform.h"

Transform::Transform() {
	// Initialize M to identity matrix, and 0 children
	M = glm::mat4(1);
}

void Transform::draw(const glm::mat4& C) {
	M = C * M;
	// Loop through all children and call their draw's
	for (Node* child : children) {
		// Pass in transform matrix so that child knows where to be relative to this transform
		child->draw(M);
	}
}

void Transform::addChild(Node* child) {
	children.push_back(child);
}

void Transform::update() {

}