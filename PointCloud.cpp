#include "PointCloud.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
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
				points.push_back(point);
				glm::vec4 homogPoint(point, 1.0f);
				testPoints.push_back(homogPoint);
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

	std::cout << "Number of points read for object " << objFilename << ": " << points.size() << std::endl;
	std::cout << "Number of vertex norms read for object " << objFilename << ": " << vertexNorms.size() << std::endl;
	std::cout << "Number of faces read for object " << objFilename << ": " << indices.size() << std::endl;

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
	// Find the min and max coords for each dimension for each vertex
	float x_min = points[0].x, y_min = points[0].y, z_min = points[0].z;
	float x_max = points[0].x, y_max = points[0].y, z_max = points[0].z;
	for (int i = 0; i < points.size(); i++) {
		x_max = std::max(x_max, points[i].x);
		y_max = std::max(y_max, points[i].y);
		z_max = std::max(z_max, points[i].z);
		x_min = std::min(x_min, points[i].x);
		y_min = std::min(y_min, points[i].y);
		z_min = std::min(z_min, points[i].z);
	}

	// Find the center point of the model (half way between the minimum and maximum values) and shift
	float x_half = (x_max + x_min) / 2;
	float y_half = (y_max + y_min) / 2;
	float z_half = (z_max + z_min) / 2;

	// Finding the max length in the x, y, z direction and scale down to fit in 1x1x1 bounding box
	float max_len = std::max(std::max(x_max - x_min, y_max - y_min), z_max - z_min);
	float scaling_factor = 14;

	for (int i = 0; i < points.size(); i++) {
		points[i].x -= x_half;
		points[i].y -= y_half;
		points[i].z -= z_half;

		// Scale down model to fit in 1x1x1 bounding box
		points[i].x /= max_len;
		points[i].y /= max_len;
		points[i].z /= max_len;

		// Rescale by scaling the 1x1x1 bounding box by some scaling factor
		points[i].x *= scaling_factor;
		points[i].y *= scaling_factor;
		points[i].z *= scaling_factor;
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)* indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	// glPointSize(pointSize);

	// Draw the points 
	// glDrawArrays(GL_POINTS, 0, points.size());
	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

glm::vec3 PointCloud::lerp(glm::vec3 start, glm::vec3 end, float t) {
	return start * (1.0f - t) + end * t;
}

void PointCloud::update()
{
	// Spin the cube by 1 degree
	// spin(0.1f);
}

void PointCloud::updateModelSize(double yoffset) {
	/*
	Scaling matrix form:
	[
		x 0 0 0
		0 y 0 0
		0 0 z 0
		0 0 0 1
	], where x, y, and z are the scaling factors for each axis
	*/
	if (yoffset > 0) {
		// Loop through each point and scale it
		for (int i = 0; i < points.size(); i++) {
			// Create model vector for the given point
			/*
			glm::vec4 model(points[i].x, points[i].y, points[i].z, 1.0f);
			glm::mat4 scaleMatrix = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
			glm::vec4 result = scaleMatrix * model;
			std::cout << glm::to_string(result) << std::endl;
			points[i] = result;
			*/
		}
	}
}


void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	if (pointSize + size <= 0) {
		std::cerr << "Attempting to update pointSize to <= 0: " << pointSize + size << endl;
	}
	else {
		pointSize += size;
		std::cout << "new point size: " << pointSize << endl;
	}
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
