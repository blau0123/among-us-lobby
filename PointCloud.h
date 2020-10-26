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
	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::ivec3> indices;

	GLuint VAO, VBO, EBO;
	GLfloat pointSize;

public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float t);
	void updatePointSize(GLfloat size);
	void spin(float deg);
};

#endif
