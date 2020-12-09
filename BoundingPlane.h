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

#include "Node.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "stb_image.h"

using namespace std;

class BoundingPlane : public Node
{
private:
	glm::vec3 position;
	glm::vec3 normal;
	std::list<Node*> children;

public:
	BoundingPlane(glm::vec3 norm, glm::vec3 pos);
	float getHeight();
	float getWidth();
	glm::vec3 getNormal();
	glm::vec3 getPosition();
	void setPosition(glm::mat4& translation);
	void addChild(Node* child);

	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader);
	void update();
};

#endif
