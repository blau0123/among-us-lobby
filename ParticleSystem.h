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

	bool active;
	float timeSystemAlive = 0;
	GLuint VAO, VBO;
	float numAliveParticles = MAX_PARTICLES;
	glm::vec3 center;
	glm::vec3 systemColor;
	std::vector<Particle> particles;
	std::vector<Particle> deadParticles;
	// Vector to hold position of all particles, will be stored in VBO --> will hold up to MAX_PARTICLES
	std::vector<glm::vec3> positions;

public:
	ParticleSystem(glm::vec3 systemPos, glm::vec3 initColor);
	~ParticleSystem();

	// Used when we want to reuse a particle that had died
	void respawnParticle(Particle p, float offset);
	// Return the index for the first dead particle found (to respawn it)
	int findFirstDeadParticle();
	bool getIfActive();
	void activateParticleSystem();
	void setColorOfParticleSystem(glm::vec3 color);

	void update(float deltaTime);
	void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLuint shader);
};

#endif
