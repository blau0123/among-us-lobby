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

public:
	AmongUsObject(std::string filename, int bfCull, int useTexture, int useToonShading);
	~AmongUsObject();

	// Note: uses Geometry's draw method
	void update();

	BoundingSphere* getBoundingSphere();
	void setBoundingSphere(float r, glm::vec3 pos);
};

#endif