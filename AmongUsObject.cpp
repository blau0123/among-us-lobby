#include "AmongUsObject.h";

#include <fstream>
#include <sstream>
#include <iostream>

AmongUsObject::AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading)
	: Geometry(filename, bfCull, useTexture, useToonShading)
{
	// Using vertices parsed from Geometry's ctor to calculate center point and
	// bounding box radius
	// Find the min and max coords for each dimension for each vertex
	float x_min = vertices[0].x, y_min = vertices[0].y, z_min = vertices[0].z;
	float x_max = vertices[0].x, y_max = vertices[0].y, z_max = vertices[0].z;
	for (int i = 0; i < vertices.size(); i++) {
		x_max = std::max(x_max, vertices[i].x);
		y_max = std::max(y_max, vertices[i].y);
		z_max = std::max(z_max, vertices[i].z);
		x_min = std::min(x_min, vertices[i].x);
		y_min = std::min(y_min, vertices[i].y);
		z_min = std::min(z_min, vertices[i].z);
	}

	// Find the center point of the model (half way between the minimum and maximum values)
	float x_half = (x_max + x_min) / 2;
	float y_half = (y_max + y_min) / 2;
	float z_half = (z_max + z_min) / 2;
	glm::vec3 objCenter(x_half, y_half, z_half);

	// Finding the max length in the x, y, z direction --> will become radius of bounding sphere
	float max_len = std::max(std::max(x_max - x_min, y_max - y_min), z_max - z_min);

	std::cout << "Setting bounding sphere for '" << filename << "' with params: r = " << max_len << ", center = (" <<
		x_half << "," << y_half << "," << z_half << ")" << std::endl;
	setBoundingSphere(max_len, objCenter);
}

AmongUsObject::~AmongUsObject() {
	delete boundingSphere;
}

void AmongUsObject::update()
{
}

void AmongUsObject::setBoundingSphere(float r, glm::vec3 pos) {
	boundingSphere = new BoundingSphere(r, pos);
}

BoundingSphere* AmongUsObject::getBoundingSphere() {
	return boundingSphere;
}
