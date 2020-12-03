#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "Geometry.h"

#include <vector>
#include <string>
#include "stb_image.h"

using namespace std;

class Lobby : public Geometry
{
private:
	glm::vec3 lastCursorPos;

	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;

public:
	Lobby(std::string filename, int bfCull, int useTexture, int useToonShading);

	// Note: uses Geometry's draw method
	void update();

	// For trackball rotation of lobby about its center
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif
