#include "BoundingSphere.h";

BoundingSphere::BoundingSphere(float r, glm::vec3 pos) {
	radius = r;
	position = pos;
}

void BoundingSphere::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader) {
	// Loop through all children and call their draw's
	for (Node* child : children) {
		// Pass in transform matrix so that child knows where to be relative to this transform
		child->draw(C, view, projection, viewDir, shader);
	}
}

void BoundingSphere::update() {
}

void BoundingSphere::addChild(Node* child) {
	children.push_back(child);
}

float BoundingSphere::getRadius() {
	return radius;
}

glm::vec3 BoundingSphere::getPosition() {
	return position;
}

void BoundingSphere::setPosition(glm::mat4& translation) {
	position = translation * glm::vec4(position, 1.0f);
}

bool BoundingSphere::transformSphere(const glm::mat4& C) {
	position = C * glm::vec4(position, 1.0f);
	return true;
}

bool BoundingSphere::detectCollision(BoundingSphere* otherSphere) {
	// Algorithm: if the sum of the squared radii > squared distance between the center points,
	// then they intersect
	float thisRadiusSqrd = radius * radius;
	float otherRadiusSqrd = otherSphere->getRadius() * otherSphere->getRadius();
	glm::vec3 pos = otherSphere->getPosition();
	//std::cout << "position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//std::cout << "sphere position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

	float distX = position.x - (otherSphere->getPosition()).x;
	distX *= distX;
	float distY = position.y - (otherSphere->getPosition()).y;
	distY *= distY;
	float distZ = position.z - (otherSphere->getPosition()).z;
	distZ *= distZ;

	float sqrdDist = distX + distY + distZ;
	//std::cout << "squard dist: " << sqrdDist << "sqrd radius sum: " << thisRadiusSqrd + otherRadiusSqrd << std::endl;

	if ((thisRadiusSqrd + otherRadiusSqrd) > sqrdDist)
		return true;
	else
		return false;
}