#include "BoundingSphere.h";

BoundingSphere::BoundingSphere(float r, glm::vec3 pos) {
	radius = r;
	position = pos;
}

bool BoundingSphere::detectCollision(BoundingSphere otherSphere) {
	// Algorithm: if the sum of the squared radii > squared distance between the center points,
	// then they intersect
	float thisRadiusSqrd = radius * radius;
	float otherRadiusSqrd = otherSphere.radius * otherSphere.radius;

	float distX = position.x - otherSphere.position.x;
	distX *= distX;
	float distY = position.y - otherSphere.position.y;
	distY *= distY;
	float distZ = position.z - otherSphere.position.z;
	distZ *= distZ;

	float sqrdDist = distX + distY + distZ;
	
	if ((thisRadiusSqrd + otherRadiusSqrd) > sqrdDist)
		return true;
	else
		return false;
}