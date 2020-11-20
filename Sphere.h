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
	GLuint vao, vboVertex, vboNormals, eboIndex;
	int lats, longs;
	int numEltsToDraw;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> norms;

public:
	Sphere();
	~Sphere();

	void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, unsigned int cubemapTextureID, GLuint shader);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void init();
	void update();
};

#endif
