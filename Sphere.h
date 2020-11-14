#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object.h"

#include <vector>
#include <string>

using namespace std;

class Sphere : public Object
{
private:
	bool isInited;
	GLuint m_vao, m_vboVertex, m_vboNormals, m_eboIndex;
	int lats, longs;
	int numEltsToDraw;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> indices;
	std::vector<glm::vec3> vertexNorms;

public:
	Sphere();
	~Sphere();

	void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, unsigned int cubemapTextureID, GLuint shader);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void init();
	void update();
};

#endif
