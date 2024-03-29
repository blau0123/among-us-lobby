#include "Sphere.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Sphere::Sphere()
{
	int i, j;
	int indicator = 0;

	for (i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1.0) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1.0) / longs;
			double lng1 = 2 * glm::pi<double>() * (double)(j) / longs;
			double x1 = cos(lng1);
			double y1 = sin(lng1);
			double x = cos(lng);
			double y = sin(lng);

			// Calculate the 4 vertices for the quad
			/*
				 x		x1
			y -> *		*

			y1 -> *		*
			*/
			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x1 * zr0);
			vertices.push_back(y1 * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x1 * zr1);
			vertices.push_back(y1 * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;

			// Calculate the avg of each axis, so then can get the normal for the entire quad, not just for each vertex
			float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
			float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
			float z_avg = (2 * z0 + 2 * z1) / 4;
			glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

			// Each vertex for a single quad will have the same normal
			norms.push_back(avg.x);
			norms.push_back(avg.y);
			norms.push_back(avg.z);

			norms.push_back(avg.x);
			norms.push_back(avg.y);
			norms.push_back(avg.z);

			norms.push_back(avg.x);
			norms.push_back(avg.y);
			norms.push_back(avg.z);

			norms.push_back(avg.x);
			norms.push_back(avg.y);
			norms.push_back(avg.z);
		}

		indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}


	// Initialize model view matrix
	model = glm::mat4(1);
	// Scale the sphere so it's bigger on the screen
	model = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)) * model;
	model = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * model;
	model = glm::translate(glm::vec3(0.0f, 5.0f, 0.0f)) * model;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(GLfloat), &norms[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &eboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	numEltsToDraw = indices.size();
	std::cout << "Finished initing sphere with " << numEltsToDraw << " number of elements" << std::endl;
}

Sphere::~Sphere()
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &vboNormals);
	glDeleteBuffers(1, &vboVertex);
	glDeleteBuffers(1, &eboIndex);
	glDeleteVertexArrays(1, &vao);
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {

}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, unsigned int cubemapTextureID, GLuint shader)
{
	// Activate the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Set the camera position to be the eyePos vector
	glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));
	// Send uniform skybox data to shader for reflections
	glUniform1i(glGetUniformLocation(shader, "skybox"), 0);

	// Bind the VAO
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndex);
	glDrawElements(GL_QUADS, numEltsToDraw, GL_UNSIGNED_INT, NULL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glDisable(GL_PRIMITIVE_RESTART);
	glUseProgram(0);
}

void Sphere::update()
{
}

