#ifndef _AMONG_US_OBJECT_H_
#define _AMONG_US_OBJECT_H_

#include "Geometry.h"
#include "BoundingSphere.h"

#include <vector>
#include <string>
#include "stb_image.h"

using namespace std;

class AmongUsObject : public Geometry
{
private:
	glm::vec3 lastCursorPos;

	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;

	// Bounding sphere for this astronaut to  check for collisions
	BoundingSphere boundingSphere;

public:
	AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading);

	void update();

	void transform(glm::mat4 transformMatrix);

	// For trackball rotation of lobby about its center
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif