#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>
#include <iostream>
#include "stb_image.h"

using namespace std;

class BoundingSphere
{
private:
	float radius;
	glm::vec3 position;

public:
	BoundingSphere(float r, glm::vec3 pos);
	bool detectCollision(BoundingSphere* otherSphere);
	float getRadius();
	glm::vec3 getPosition();
	bool transformSphere(const glm::mat4& C);
};

#endif
