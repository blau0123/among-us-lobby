#include "PointCloud.h"
#include <fstream>
#include <sstream>
#include <iostream>

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
			}
		}
	}
	else {
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close();

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

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
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
	glPointSize(pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, points.size());

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
	spin(0.1f);
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
