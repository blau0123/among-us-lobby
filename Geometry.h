#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"

#include <vector>
#include <string>
#include <list>
#include "stb_image.h"

using namespace std;

/*
Common base class for all nodes in the Scene Graph.
*/
class Geometry : public Node
{
protected:
	// Model view matrix of this object
	glm::mat4 model;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::ivec3> indices;

	// These will hold vertices and normals with the same ordering, so therefore we don't need an EBO for indices
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_normals;
	std::vector<glm::vec2> out_tex;
	std::vector<glm::ivec3> out_indices;

	// Used to determine whether to use Phong illumination, or to just assign a color (no use normals)
	int isGround = 0;

	GLuint textureId;

	glm::vec3 k_diffuse;
	glm::vec3 k_specular;
	glm::vec3 k_ambient;
	float shininess;

	// If = 0, don't map object to a texture; if = 1, map to texture
	int useTex;
	int useToon;

	std::string name;
	// 0 if this geometry requires backface culling, or 1 if not (such as cylinder, where want to show the backface)
	int requiresBackfaceCull = 0;

	GLuint vao, vboVertex, vboNormals, vbo_t, eboIndices;

public:
	Geometry(std::string filename, int bfCull, int useTexture, int useToonShading);
	~Geometry();
	// Calculate the object's coordinates relative to parent using C and modelview matrix 
	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader);
	// Load OBJ file
	void init(std::string filename);
	void setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s);
	GLuint loadTexture(std::string texLocation);
	// Used to separate bounding sphere updates from rendering
	void update();
};

#endif