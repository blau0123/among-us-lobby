#include "BoundingPlane.h";

BoundingPlane::BoundingPlane(glm::vec3 norm, glm::vec3 pos) {
	normal = norm;
	position = pos;
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
