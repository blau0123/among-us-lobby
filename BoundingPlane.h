#ifndef _BOUNDING_PLANE_H_
#define _BOUNDING_PLANE_H_

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
#include <list>
#include "stb_image.h"

using namespace std;

class BoundingPlane 
{
private:
	glm::vec3 position;
	glm::vec3 normal;

public:
	BoundingPlane(glm::vec3 norm, glm::vec3 pos);
	glm::vec3 getNormal();
	glm::vec3 getPosition();
	void setPosition(glm::mat4& translation);
};

#endif
