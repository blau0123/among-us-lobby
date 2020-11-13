#ifndef _NODE_H_
#define _NODE_H_

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
private:


public:
	virtual void draw(const glm::mat4& C) = 0;
	// Used to separate bounding sphere updates from rendering
	virtual void update() = 0;
};

#endif
