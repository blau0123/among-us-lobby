#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "Object.h"

#include <vector>
#include <string>
#include "stb_image.h"

using namespace std;

class AmongUsObject : public Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::ivec3> indices;

	// These will hold vertices and normals with the same ordering, so that vertices and normals are matched
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_normals;
	std::vector<glm::vec2> out_tex;
	std::vector<glm::ivec3> out_indices;

	glm::vec3 lastCursorPos;

	GLuint textureId;

	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;

	// vbo_v holds position (vertex) data, vbo_n holds vertex normal data, ebo holds index data
	GLuint vao, vbo_v, vbo_n, vbo_t, ebo;

public:
	AmongUsObject(std::string filename);
	~AmongUsObject();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s);

	GLuint loadTexture(std::string texLocation);

	// For trackball rotation of lobby about its center
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif