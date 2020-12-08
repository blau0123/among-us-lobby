#include "BoundingSphere.h";

BoundingSphere::BoundingSphere(float r, glm::vec3 pos) {
	radius = r;
	position = pos;
}

float BoundingSphere::getRadius() {
	return radius;
}

glm::vec3 BoundingSphere::getPosition() {
	return position;
}

bool BoundingSphere::detectCollision(BoundingSphere* otherSphere) {
	// Algorithm: if the sum of the squared radii > squared distance between the center points,
	// then they intersect
	float thisRadiusSqrd = radius * radius;
	float otherRadiusSqrd = otherSphere->getRadius() * otherSphere->getRadius();

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