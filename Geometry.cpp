#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>

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
	// Hold the indices for vertices and normals in order to get a vertices and normals vector with same ordering
	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;

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
				int lastX = 0;
				int lastY = 0;
				int lastZ = 0;
				ss >> pairsX >> pairsY >> pairsZ;
				// Subtract one because indices start at 1, so will be one off of the real vertex indices
				faceIndices.x = std::stoi(pairsX.substr(0, pairsX.find("/"))) - 1;
				faceIndices.y = std::stoi(pairsY.substr(0, pairsY.find("/"))) - 1;
				faceIndices.z = std::stoi(pairsZ.substr(0, pairsZ.find("/"))) - 1;
				indices.push_back(faceIndices);

				vertexIndices.push_back(std::stoi(pairsX.substr(0, pairsX.find("/", lastX))));
				vertexIndices.push_back(std::stoi(pairsY.substr(0, pairsY.find("/", lastY))));
				vertexIndices.push_back(std::stoi(pairsZ.substr(0, pairsZ.find("/", lastZ))));
				lastX = pairsX.find("/", lastX) + 1;
				lastY = pairsY.find("/", lastY) + 1;
				lastZ = pairsZ.find("/", lastZ) + 1;
				normalIndices.push_back(std::stoi(pairsX.substr(pairsX.find("/", lastX) + 1)));
				normalIndices.push_back(std::stoi(pairsY.substr(pairsY.find("/", lastY) + 1)));
				normalIndices.push_back(std::stoi(pairsZ.substr(pairsZ.find("/", lastZ) + 1)));
			}
		}
	}
	else {
		std::cerr << "Can't open the file " << filename << std::endl;
	}

	objFile.close();

	// For each vertex of each triangle, add the vertex position with it's corresponding normal
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the vertex and normal from the given index
		int vertexIndex = vertexIndices[i];
		int normIndex = normalIndices[i];
		glm::vec3 vertex = vertices[vertexIndex - 1];
		glm::vec3 normal = vertexNorms[vertexIndex - 1];
		out_vertices.push_back(vertex);
		out_normals.push_back(normal);
	}

	for (unsigned int i = 0; i < vertexIndices.size() - 2; i+=3) {
		glm::ivec3 index;
		index.x = i;
		index.y = i + 1;
		index.z = i + 2;
		out_indices.push_back(index);
	}

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
	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * out_vertices.size(), &out_vertices[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO2 to the bound VAO, and store the vertex norm data
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNorms.size(), vertexNorms.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * out_normals.size(), &out_normals[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(1);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &eboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)* out_indices.size(), &out_indices[0], GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, GLuint shader) {
	// Apply the parent's transformations to the Geometry's model so that it is in the correct position relative to the parent
	model = C;
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniform3fv(glGetUniformLocation(shader, "k_diffuse"), 1, glm::value_ptr(k_diffuse));
	glUniform3fv(glGetUniformLocation(shader, "k_specular"), 1, glm::value_ptr(k_specular));
	glUniform3fv(glGetUniformLocation(shader, "k_ambient"), 1, glm::value_ptr(k_ambient));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);
	glUniform1i(glGetUniformLocation(shader, "isLightSource"), 0);

	// Pass in which render mode we are in (normal, Phong)
	glUniform1i(glGetUniformLocation(shader, "render_mode"), 1);

	// Bind the VAO
	glBindVertexArray(vao);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Geometry::transform(glm::mat4 transformMatrix) {
	model = transformMatrix * model;
}

void Geometry::setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s) {
	// Set the uniform variable for k for diffuse, specular, ambient for this specific object
	k_diffuse = k_d;
	k_specular = k_s;
	k_ambient = k_a;
	shininess = s;
}

void Geometry::update() {

}