#include "BoundingPlane.h";

BoundingPlane::BoundingPlane(glm::vec3 norm, glm::vec3 pos) {
	normal = norm;
	position = pos;
}

void BoundingPlane::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader) {
	// Loop through all children and call their draw's
	for (Node* child : children) {
		// Pass in transform matrix so that child knows where to be relative to this transform
		child->draw(C, view, projection, viewDir, shader);
	}
}

void BoundingPlane::update() {
}

void BoundingPlane::addChild(Node* child) {
	children.push_back(child);
}

glm::vec3 BoundingPlane::getNormal() {
	return normal;
}

glm::vec3 BoundingPlane::getPosition() {
	return position;
}

void BoundingPlane::setPosition(glm::mat4& translation) {
	position = translation * glm::vec4(position, 1.0f);
}
