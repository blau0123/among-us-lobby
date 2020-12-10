#ifndef _BOUNDING_SHAPE_H_
#define _BOUNDING_SHAPE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Node.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "stb_image.h"

using namespace std;

class BoundingShape 
{
public:
	virtual glm::vec3 getPosition() = 0;
	virtual void setPosition(glm::mat4& translation) = 0;
};

#endif
