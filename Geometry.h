#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

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

	glm::vec3 k_diffuse;
	glm::vec3 k_specular;
	glm::vec3 k_ambient;
	float shininess;

	GLuint vao, vboVertex, vboNormals, eboIndices;

public:
	Geometry(std::string filename);
	~Geometry();
	// Calculate the object's coordinates relative to parent using C and modelview matrix 
	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	// Load OBJ file
	void init(std::string filename);
	void setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s);
	// Used to transform the object separately, such as scaling, rotating, or translating
	void transform(glm::mat4 transformMatrix);
	// Used to separate bounding sphere updates from rendering
	void update();
};

#endif