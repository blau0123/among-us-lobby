#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"

#include <vector>
#include <string>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec4> testPoints;

	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::ivec3> indices;

	glm::vec3 lastCursorPos;

	// Determines if we are rotating the object or not --> 0 = ROTATE
	int movement;

	GLuint VAO, VBO, EBO;
	GLfloat pointSize;

public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float t);
	void updatePointSize(GLfloat size);
	void updateModelSize(double yoffset);
	void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	void endRotateModel();
	glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	void spin(float deg);
};

#endif
