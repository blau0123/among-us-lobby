#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <vector>
#include <string>
#include <list>

using namespace std;

/*
Common base class for all nodes in the Scene Graph.
*/
class Transform : public Node
{
private:
	// Transformation matrix
	glm::mat4 M;
	std::list<Node*> children;

public:
	Transform();
	// Will traverse the list of children and call each child node's draw function
	// When draw(C) is called, multiply matrix M with matrix C
	void draw(const glm::mat4& C);
	void addChild(Node* child);
	// Used to separate bounding sphere updates from rendering
	void update();
};

#endif