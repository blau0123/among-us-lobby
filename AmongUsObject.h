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
	// Bounding sphere for this astronaut to  check for collisions
	BoundingSphere* boundingSphere;
	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;
	glm::vec3 lastCursorPos;

public:
	AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading);
	~AmongUsObject();

	// Note: uses Geometry's draw method
	void update();
	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader);

	BoundingSphere* getBoundingSphere();
	void setBoundingSphere(float r, glm::vec3 pos);

	// For trackball rotation of lobby about its center
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif