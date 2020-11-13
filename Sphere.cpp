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

void Sphere::init(std::string objFilename) {
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
			double x = cos(lng);
			double y = sin(lng);

			vertices1.push_back(x * zr0);
			vertices1.push_back(y * zr0);
			vertices1.push_back(z0);
			//vertexNorms.push_back(glm::normalize(glm::vec3(x * zr0, y * zr0, z0)));
			indices1.push_back(indicator);
			indicator++;

			vertices1.push_back(x * zr1);
			vertices1.push_back(y * zr1);
			vertices1.push_back(z1);
			// get 4 quad points: if start long = 0, lat = 0, get right point by long + 1, lat + long to get bottom left, lat + long + 1 to get bottom right
			// get positions of each point, then get difference vectors between 0,0; 0, long+1; 0, 1
			// cross product of the difference vectors to get perpendicular vector
			//vertexNorms.push_back(glm::normalize(glm::vec3(x * zr1, y * zr1, z1)));
			indices1.push_back(indicator);
			indicator++;
		}

		indices1.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}


	// Initialize model view matrix
	model = glm::mat4(1);
	// Scale the sphere so it's bigger on the screen
	model = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f)) * model;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vboVertex);
	glGenBuffers(1, &m_vboNormals);

	// Bind VAO
	glBindVertexArray(m_vao);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices1.size(), &vertices[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Bind VBO2 to the bound VAO, and store the vertex norm data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNorms.size(), vertexNorms.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(1);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &m_eboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * indices1.size(), &indices1[0], GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	numEltsToDraw = indices1.size();
	std::cout << "Finished initing sphere with " << numEltsToDraw << " number of elements" << std::endl;
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {

}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Set the camera position to be the eyePos vector
	glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));
	// Send uniform skybox data to shader for reflections
	glUniform1i(glGetUniformLocation(shader, "skybox"), 0);
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(m_vao);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
	glDrawElements(GL_QUAD_STRIP, numEltsToDraw, GL_UNSIGNED_INT, 0);
	// Draw the points using triangles, indexed with the EBO
	// Somehow needs to bind the texture cube map to the id of the cubemap from Cube.cpp
	// glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
	// glDrawElements(GL_TRIANGLES, numEltsToDraw, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sphere::update()
{
}

