#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <ctime>

#include "stb_image.h"

using namespace std;

class ParticleSystem
{
private:
	struct Particle {
		glm::vec3 velocity, position, color;
		// If life <= 0, then this particle is "dead" and we shouldn't render it
		float life;
		// Set initial position and velocity of this specific particle
		void init(glm::vec3 initVelocity, glm::vec3 initPos, glm::vec3 initColor) {
			velocity = initVelocity;
			position = initPos;
			color = initColor;
		};
		// Run one time-step
		void update(float deltaTime);
	};
	static const int MAX_PARTICLES = 200;
	static const int PARTICLE_LIFE = 3;

	GLuint VAO, VBO;
	Particle particles[MAX_PARTICLES];
	// Vector to hold position of all particles, will be stored in VBO --> will hold up to MAX_PARTICLES
	std::vector<glm::vec3> positions;
	glm::vec3 position_data[MAX_PARTICLES];

public:
	ParticleSystem(glm::vec3 systemPos);
	~ParticleSystem();

	void update(float deltaTime);
	void draw(GLuint shader, glm::vec3 cameraPos, glm::mat4 model);
};

#endif
