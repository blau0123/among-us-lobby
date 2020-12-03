#include "BoundingSphere.h";

BoundingSphere::BoundingSphere(float r, glm::vec3 pos) {
	radius = r;
	position = pos;
}

bool BoundingSphere::detectCollision(BoundingSphere otherSphere) {
	return true;
}