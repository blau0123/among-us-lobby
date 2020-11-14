#include "Sphere.h"

#include <fstream>
#include <sstream>
#include <iostream>

Sphere::Sphere()
{
	isInited = false;
	m_vao = 0;
	// 1 vbo to store vertices of sphere, 1 vbo to hold normals, 1 ebo to hold indices in order to draw faces
	m_vboVertex = 0;
	m_vboNormals = 0;
	m_eboIndex = 0;

	// Along longitude, have 40 quads, and along latitude, have 40 quads
	lats = 40;
	longs = 40;
}

Sphere::~Sphere()
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &m_vboNormals);
	glDeleteBuffers(1, &m_eboIndex);
	glDeleteBuffers(1, &m_eboIndex);
	glDeleteVertexArrays(1, &m_vao);
}

void Sphere::init() {
	int i, j;
	std::vector<GLfloat> vertices1;
	std::vector<GLuint> indices1;
	std::vector<GLfloat> norms1;
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
			vertices1.push_back(x * zr1);
			vertices1.push_back(y * zr1);
			vertices1.push_back(z1);
			indices1.push_back(indicator);
			indicator++;

			vertices1.push_back(x * zr0);
			vertices1.push_back(y * zr0);
			vertices1.push_back(z0);
			indices1.push_back(indicator);
			indicator++;

			vertices1.push_back(x1 * zr0);
			vertices1.push_back(y1 * zr0);
			vertices1.push_back(z0);
			indices1.push_back(indicator);
			indicator++;

			vertices1.push_back(x1 * zr1);
			vertices1.push_back(y1 * zr1);
			vertices1.push_back(z1);
			indices1.push_back(indicator);
			indicator++;

			// Calculate the avg of each axis, so then can get the normal for the entire quad, not just for each vertex
			float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
			float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
			float z_avg = (2 * z0 + 2 * z1) / 4;
			glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

			// Each vertex for a single quad will have the same normal
			norms1.push_back(avg.x);
			norms1.push_back(avg.y);
			norms1.push_back(avg.z);

			norms1.push_back(avg.x);
			norms1.push_back(avg.y);
			norms1.push_back(avg.z);

			norms1.push_back(avg.x);
			norms1.push_back(avg.y);
			norms1.push_back(avg.z);

			norms1.push_back(avg.x);
			norms1.push_back(avg.y);
			norms1.push_back(avg.z);
		}

		indices1.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}


	// Initialize model view matrix
	model = glm::mat4(1);
	// Scale the sphere so it's bigger on the screen
	// model = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)) * model;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vboVertex);
	glGenBuffers(1, &m_vboNormals);

	// Bind VAO
	glBindVertexArray(m_vao);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices1.size(), &vertices1[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Bind VBO2 to the bound VAO, and store the vertex norm data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * norms1.size(), &norms1[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(1);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &m_eboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * indices1.size(), &indices1[0], GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	numEltsToDraw = indices1.size();
	std::cout << "Finished initing sphere with " << numEltsToDraw << " number of elements" << std::endl;
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
	glBindVertexArray(m_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
	glDrawElements(GL_QUADS, numEltsToDraw, GL_UNSIGNED_INT, NULL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sphere::update()
{
}

