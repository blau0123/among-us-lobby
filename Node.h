#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

using namespace std;

/*
Common base class for all nodes in the Scene Graph.
*/
class Node 
{
public:
	virtual void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, GLuint shader) = 0;
	// Used to separate bounding sphere updates from rendering
	virtual void update() = 0;
};

#endif
