#include "AmongUsObject.h";

#include <fstream>
#include <sstream>
#include <iostream>

AmongUsObject::AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading)
	: Geometry(filename, bfCull, useTexture, useToonShading)
{
	movement = -1;
	lastCursorPos = glm::vec3(0.0f);

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
	setBoundingSphere(max_len / 2.0f, objCenter);
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


void AmongUsObject::initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	// Turn on user interactive rotations
	movement = 0;

	// Map the mouse position to a logical sphere location and keep track of the last known mouse position
	lastCursorPos = trackBallMapping(windowWidth, windowHeight, cursorPos);

	// Set the OpenGL state to modify the MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void AmongUsObject::rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos) {
	glm::vec3 currSpherePos, rotateDirection;
	float rot_angle;

	// Check which type of movement we are doing
	if (movement == 0) {
		// Map this mouse position to it's logical sphere location
		currSpherePos = trackBallMapping(windowWidth, windowHeight, currCursorPos);

		// Determine the direction that the object should be rotated in logical sphere
		rotateDirection = currSpherePos - lastCursorPos;
		float velocity = glm::length(rotateDirection);

		// Only rotate if there is a decent amount of movement
		if (velocity > 0.0001f) {
			// Rotate about the axis that is perpendicular to the two points
			glm::vec3 rotAxis = glm::cross(lastCursorPos, currSpherePos);
			rot_angle = velocity * 40;

			// Calculate the rotation matrix
			model = model * glm::rotate(glm::radians(rot_angle), rotAxis);
		}

		// Save the current point location for the next movement
		lastCursorPos = currSpherePos;
	}
}

void AmongUsObject::endRotateModel() {
	// Turn off rotation
	movement = -1;
}

// Calculate 3D position of a projected unit vector onto the xy-plane
glm::vec3 AmongUsObject::trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * cursorPos.x - windowWidth) / windowWidth;
	v.y = (windowHeight - 2.0 * cursorPos.y) / windowHeight;
	v.z = 0.0f;
	d = glm::length(v);
	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrtf(1.001f - d * d);
	v = glm::normalize(v);
	return v;
}
