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
	// faces: ((v_x, vn_x), (v_y, vn_y), (v_z, vn_z)) <-- vector<glm::vec2> (1 element of faces)
	// faces will contain the indices to those vertex-vertex normal pairs
	std::vector<vector<glm::vec2>> faces;

	GLuint VAO, VBO;
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
