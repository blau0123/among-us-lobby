#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "Node.h"

#include <vector>
#include <string>
#include <list>

using namespace std;

/*
Common base class for all nodes in the Scene Graph.
*/
class Geometry : public Node
{
private:
	// Model view matrix of this object
	glm::mat4 model;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::ivec3> indices;

	GLuint vao, vboVertex, vboNormals, eboIndices;

public:
	Geometry(std::string filename);
	~Geometry();
	// Calculate the object's coordinates relative to parent using C and modelview matrix 
	void draw(const glm::mat4& C);
	// Load OBJ file
	void init(std::string filename);
	// Used to separate bounding sphere updates from rendering
	void update();
};

#endif