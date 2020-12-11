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

#include "BoundingPlane.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "stb_image.h"

using namespace std;

// BoundingSphere acts as a transform node, where it calls draw() for all children and all
// children represent the object bounded by this BoundingSphere
class BoundingSphere 
{
private:
	float radius;
	glm::vec3 position;

public:
	BoundingSphere(float r, glm::vec3 pos);
	float detectCollision(BoundingSphere* otherSphere);
	float detectCollisionWithWall(BoundingPlane* wall);
	float getRadius();
	glm::vec3 getPosition();
	void setPosition(glm::mat4& translation);
	void setPositionToVector(glm::vec3 newPos);
	bool transformSphere(const glm::mat4& C);
};

#endif
