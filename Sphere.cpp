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
	std::ifstream objFile(objFilename);
	// Check whether the file can be opened before reading
	if (objFile.is_open()) {
		// Get one line at a time
		std::string line;
		while (std::getline(objFile, line)) {
			// Turn the line into a string stream for processing
			std::stringstream ss;
			ss << line;

			// Read the first word of the line
			std::string label;
			ss >> label;

			// If the line is about vertex (start with a v), then read the content of the line
			if (label == "v") {
				// Read the float numbers (the next three words) and use them as the coordinates for the points
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				// Process the point (save it)
				vertices.push_back(point);
			}
			else if (label == "vn") {
				// Like the vertex, read the float numbers (next three words) and use them as the vertex norm coordinates
				glm::vec3 vNorm;
				ss >> vNorm.x >> vNorm.y >> vNorm.z;
				vertexNorms.push_back(vNorm);
			}
			else if (label == "f") {
				glm::ivec3 faceIndices;
				// Get the three vertex-vertex normal index pairs (the x, y, z)
				std::string pairsX, pairsY, pairsZ;
				ss >> pairsX >> pairsY >> pairsZ;
				// Subtract one because indices start at 1, so will be one off of the real vertex indices
				faceIndices.x = std::stoi(pairsX.substr(0, pairsX.find("//"))) - 1;
				faceIndices.y = std::stoi(pairsY.substr(0, pairsY.find("//"))) - 1;
				faceIndices.z = std::stoi(pairsZ.substr(0, pairsZ.find("//"))) - 1;
				indices.push_back(faceIndices);
			}
		}
	}
	else {
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close();

	// Initialize model view matrix
	model = glm::mat4(1);
	// Scale the sphere so it's bigger on the screen
	model = glm::scale(glm::vec3(2, 2, 2)) * model;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vboVertex);
	glGenBuffers(1, &m_vboNormals);

	// Bind VAO
	glBindVertexArray(m_vao);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "Finished initing sphere" << std::endl;
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(m_vao);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sphere::update()
{
}

