#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Geometry::Geometry(std::string filename) {
	init(filename);
}

Geometry::~Geometry()
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &vboVertex);
	glDeleteBuffers(1, &vboNormals);
	glDeleteBuffers(1, &eboIndices);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::init(std::string filename) {
	std::ifstream objFile(filename);
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
				faceIndices.x = std::stoi(pairsX.substr(0, pairsX.find("/"))) - 1;
				faceIndices.y = std::stoi(pairsY.substr(0, pairsY.find("/"))) - 1;
				faceIndices.z = std::stoi(pairsZ.substr(0, pairsZ.find("/"))) - 1;
				indices.push_back(faceIndices);
			}
		}
	}
	else {
		std::cerr << "Can't open the file " << filename << std::endl;
	}

	objFile.close();

	std::cout << "Number of points read for object " << filename << ": " << vertices.size() << std::endl;
	std::cout << "Number of vertex norms read for object " << filename << ": " << vertexNorms.size() << std::endl;
	std::cout << "Number of faces read for object " << filename << ": " << indices.size() << std::endl;

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertex);
	glGenBuffers(1, &vboNormals);

	// Bind VAO
	glBindVertexArray(vao);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO2 to the bound VAO, and store the vertex norm data
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNorms.size(), vertexNorms.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(1);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &eboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::draw(const glm::mat4& C) {
	// Render the object
}

void Geometry::update() {

}