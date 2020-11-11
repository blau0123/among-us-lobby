#ifndef _LIGHT_SOURCE_H_
#define _LIGHT_SOURCE_H_

#include "Object.h"

#include <vector>
#include <string>

using namespace std;

class LightSource : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec4> testPoints;

	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::ivec3> indices;

	glm::vec3 lastCursorPos;

	// Light Properties
	glm::vec3 lightPosition;
	glm::vec3 lightColor;

	// Determines which rendering mode (0 = normal coloring, 1 = Phong illumination)
	int renderMode;

	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;

	// VBO holds position (vertex) data, VBO2 holds vertex normal data
	GLuint VAO, VBO, VBO2, EBO;
	GLfloat pointSize;

public:
	LightSource(std::string objFilename, GLfloat pointSize);
	~LightSource();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float t);
	void initializeLightSourceProperties(GLuint shader, glm::vec3 viewPos);
	void updateLightPositionToCenter(double yoffset);
	void setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s);
	void updatePointSize(GLfloat size);
	void updateModelSize(double yoffset);
	void changeRenderingMode();
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void spin(float deg);
};

#endif
