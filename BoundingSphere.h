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

#include "Node.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "stb_image.h"

using namespace std;

// BoundingSphere acts as a transform node, where it calls draw() for all children and all
// children represent the object bounded by this BoundingSphere
class BoundingSphere : public Node
{
private:
	float radius;
	glm::vec3 position;
	std::list<Node*> children;

public:
	BoundingSphere(float r, glm::vec3 pos);
	bool detectCollision(BoundingSphere* otherSphere);
	float getRadius();
	glm::vec3 getPosition();
	void setPosition(glm::mat4& translation);
	bool transformSphere(const glm::mat4& C);
	void addChild(Node* child);

	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader);
	void update();
};

#endif
