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
	// True if this object is currently rendered on screen (used for when non-user
	// astronauts will appear and disappear)
	bool renderObj;
	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;
	bool isMoving;
	glm::vec3 lastCursorPos;

public:
	AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading);
	~AmongUsObject();

	// Note: uses Geometry's draw method
	void update();
	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader);

	bool shouldRenderObj();
	void setShouldRenderObj(bool r);

	bool getIsMoving();
	void setIsMoving(bool s);

	BoundingSphere* getBoundingSphere();
	void updateBoundingSphere(glm::mat4& translation);
	void setBoundingSpherePosition(glm::vec3 newPos);
	void setBoundingSphere(float r, glm::vec3 pos);

	// For trackball rotation of lobby about its center
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif