#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 systemPos) {
	// Initialize the particle system and create an instance for each particle
	// We will spawn particles in this system within 1.0f in every axis of the provided pos
	float offset = 1.0f;
	// Make the random number generator random
	srand((unsigned int)time(NULL));

	// Create all of the particles with random positions as described above
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
		glm::vec3 initVelocity(0.01f, 0.01f, 0.01f);

		Particle newParticle;
		newParticle.init(initVelocity, initPos, glm::vec3(0.0f, 0.0f, 0.0f));
		newParticle.life = 5;
		positions.push_back(initPos);
		position_data[i] = initPos;
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

void ParticleSystem::update(float deltaTime) {
	// Update each particle's life, position (based on velocity), etc.
}

void ParticleSystem::draw(GLuint shader, glm::vec3 cameraPos, glm::mat4 model) {
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	// glPointSize(pointSize);
	// Draw the points 
	// glDrawArrays(GL_POINTS, 0, positions.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}