#ifndef _CUBE_H_
#define _CUBE_H_

#include "Object.h"

#include <vector>
#include <string>

class Cube : public Object
{
private:
	GLuint VAO;
	GLuint VBO, EBO;
	unsigned int cubemapTextureID;

public:
	Cube(float size);
	~Cube();

	unsigned int loadCubemap(std::vector<std::string> faces);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void spin(float deg);
};

#endif