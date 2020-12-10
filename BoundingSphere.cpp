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

float BoundingSphere::detectCollisionWithWall(BoundingPlane* wall) {
	// Calculate the minimum distance from center of sphere with the wall
	float dist = glm::dot((position - wall->getPosition()), wall->getNormal());

	// If the distance < radius, that means there is a collision
	if (dist < radius) 
		return radius - dist;
	return -1.0f;
}

float BoundingSphere::detectCollision(BoundingSphere* otherSphere) {
	// Algorithm: if the sum of the squared radii > squared distance between the center points,
	// then they intersect
	glm::vec3 otherPos = otherSphere->getPosition();
	float otherRadius = otherSphere->getRadius();
	//std::cout << "position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//std::cout << "sphere position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

	// If the distance between the 2 centers is > r1 + r2, then colliding (dist >= 0)
	float dist = glm::length(position - otherPos) - (radius + otherRadius);
	//std::cout << "squard dist: " << sqrdDist << "sqrd radius sum: " << thisRadiusSqrd + otherRadiusSqrd << std::endl;
	if (dist < 0)
		return dist;
	return -1.0f;
}