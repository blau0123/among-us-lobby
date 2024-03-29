#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 systemPos, glm::vec3 initColor, GLuint particleSystemShader) {
	// Initialize the particle system and create an instance for each particle
	// We will spawn particles in this system within 1.0f in every axis of the provided pos
	float offset = 0.2f;
	systemShader = particleSystemShader;
	active = true;
	timeSystemAlive = 0;
	systemModel = glm::mat4(1);

	// Make the random number generator random
	srand((unsigned int)time(NULL));

	// Create all of the particles with random positions as described above
	center = systemPos;
	systemColor = initColor;
	float xPosMax = systemPos.x + offset;
	float xPosMin = systemPos.x - offset;
	float yPosMax = systemPos.y + offset;
	float yPosMin = systemPos.y - offset;
	float zPosMax = systemPos.z + offset;
	float zPosMin = systemPos.z - offset;
	for (int i = 0; i < MAX_PARTICLES; i++) {
		// Create initial, semi-random pos
		float random = ((float)rand()) / ((float)RAND_MAX);
		float randXPos = (random * (xPosMax - xPosMin)) + xPosMin;
		random = ((float)rand()) / ((float)RAND_MAX);
		float randYPos = (random * (yPosMax - yPosMin)) + yPosMin;
		random = ((float)rand()) / ((float)RAND_MAX);
		float randZPos = (random * (zPosMax - zPosMin)) + zPosMin;
		glm::vec3 initPos(randXPos, randYPos, randZPos);

		// Create initial, semi-random velocity
		random = ((float)rand()) / ((float)RAND_MAX);
		float randXVelocity = (random * (0.5 + 0.5)) - 0.5;
		random = ((float)rand()) / ((float)RAND_MAX);
		float randYVelocity = (random * (0.5 + 0.5)) - 0.5;
		random = ((float)rand()) / ((float)RAND_MAX);
		float randZVelocity = (random * (0.5 + 0.5)) - 0.5;
		glm::vec3 initVelocity(randXVelocity, randYVelocity, randZVelocity);

		Particle newParticle;
		newParticle.init(initVelocity, initPos, initColor);
		newParticle.life = 1;
		positions.push_back(initPos);
		particles.push_back(newParticle);
		//aliveParticles.push_back(newParticle);
	}

	// Store the position data in the VBO
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);
	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void ParticleSystem::respawnParticle(Particle p, float offset) {
	// Create all of the particles with random positions as described above
	float xPosMax = center.x + offset;
	float xPosMin = center.x - offset;
	float yPosMax = center.y + offset;
	float yPosMin = center.y - offset;
	float zPosMax = center.z + offset;
	float zPosMin = center.z - offset;

	// Create initial, semi-random pos
	float random = ((float)rand()) / ((float)RAND_MAX);
	float randXPos = (random * (xPosMax - xPosMin)) + xPosMin;
	random = ((float)rand()) / ((float)RAND_MAX);
	float randYPos = (random * (yPosMax - yPosMin)) + yPosMin;
	random = ((float)rand()) / ((float)RAND_MAX);
	float randZPos = (random * (zPosMax - zPosMin)) + zPosMin;
	glm::vec3 initPos(randXPos, randYPos, randZPos);

	// Create initial, semi-random velocity
	random = ((float)rand()) / ((float)RAND_MAX);
	float randXVelocity = (random * (0.5 + 0.5)) - 0.5;
	random = ((float)rand()) / ((float)RAND_MAX);
	float randYVelocity = (random * (0.5 + 0.5)) - 0.5;
	random = ((float)rand()) / ((float)RAND_MAX);
	float randZVelocity = (random * (0.5 + 0.5)) - 0.5;
	glm::vec3 initVelocity(randXVelocity, randYVelocity, randZVelocity);

	p.init(initVelocity, initPos, glm::vec3(1.0f, 0.0f, 0.0f));
	p.life = 1;

	// Add the revived particle and its new position to the particle and position vectors (index aligned)
	particles.push_back(p);
	positions.push_back(initPos);
}

int ParticleSystem::findFirstDeadParticle() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].life <= 0)
			return i;
	}

	// If all other particles are alive, return the first particle (override it)
	return 0;
}

void ParticleSystem::updateSystemModel(glm::mat4 transform) {
	systemModel = transform * systemModel;
}

bool ParticleSystem::getIfActive() {
	return active;
}

void ParticleSystem::activateParticleSystem() {
	active = true;
	timeSystemAlive = 0;
}

void ParticleSystem::setColorOfParticleSystem(glm::vec3 color) {
	systemColor = color;
	for (int i = 0; i < particles.size(); i++) {
		particles[i].color = color;
	}
}

void ParticleSystem::update(float deltaTime) {
	if (timeSystemAlive > 3.0f) {
		active = false;
		return;
	}
	timeSystemAlive += deltaTime;

	// If there are dead particles (# particles < MAX_PARTICLES), add in some more particles
	if (deadParticles.size() > 0) {
		// Make the random number generator random
		srand((unsigned int)time(NULL));
		for (int i = 0; i < deadParticles.size(); i++) {
			respawnParticle(deadParticles[i], 0.2f);
			// Remove this particle from deadParticles because was revived
			// deadParticles.erase(deadParticles.begin() + i);
		}
		deadParticles.clear();
	}
	// Update each particle's life, position (based on velocity), etc.
	for (int i = 0; i < particles.size(); i++) {
		Particle currParticle = particles[i];
		currParticle.life -= deltaTime;
		if (currParticle.life <= 0) {
			// Remove this particle from particles and positions (the indexes will be aligned)
			deadParticles.push_back(particles[i]);
			particles.erase(particles.begin() + i);
			positions.erase(positions.begin() + i);
		}
		else {
			// If particle is alive, then update it's position using the velocity
			currParticle.position -= currParticle.velocity * deltaTime;
			particles[i] = currParticle;
			positions[i] = currParticle.position;
		}
	}

	// Update the position data
	glBindVertexArray(VAO);
	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * positions.size(), &positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLuint shader) {
	// Activate the shader program 
	// Don't use the shader passed in by the Transform node, since it will use the Phong shader
	glUseProgram(shader);
	//glm::mat4 test = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(systemModel));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));

	glUniform3fv(glGetUniformLocation(shader, "particleColor"), 1, glm::value_ptr(systemColor));
	glUniform1i(glGetUniformLocation(shader, "pointSize"), 20);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	// Draw the points
	glDrawArrays(GL_POINTS, 0, positions.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}