#include "LightSource.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

LightSource::LightSource(std::string objFilename, GLfloat pointSize)
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

	// Move this light source sphere to the specified light position point
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
	glm::vec3 diff = glm::vec3(x_half, y_half, z_half) - lightPosition;

	// Initialize light source parameters
	lightPosition = glm::vec3(10.0f, 0.0f, 0.0f);
	lightColor = glm::vec3(1.0f, 0.5f, 0.31f);

	// Initialize rotation variables
	movement = -1;
	lastCursorPos = glm::vec3(0.0f);

	// Initialize render mode
	renderMode = 1;

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Move the light source to the location of the light used in the shader and scale the object down
	model = glm::translate(lightPosition) * glm::scale(glm::vec3(0.5, 0.5, 0.5)) * model;
	std::cout << "Light position: " << lightPosition.x << "," << lightPosition.y << "," << lightPosition.z << std::endl;

	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	// Location for the position layout variable in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO2 to the bound VAO, and store the vertex norm data
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNorms.size(), vertexNorms.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(1);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LightSource::~LightSource()
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void LightSource::initializeLightSourceProperties(GLuint shader, glm::vec3 viewPos) {
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform light data to the shader 
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));
	glUniform1f(glGetUniformLocation(shader, "lightLinear"), 0.09);
	std::cout << "Set the light property uniform variables" << std::endl;

	glUseProgram(0);
}

void LightSource::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	glUniform3fv(glGetUniformLocation(shader, "k_diffuse"), 1, glm::value_ptr(k_diffuse));
	glUniform3fv(glGetUniformLocation(shader, "k_specular"), 1, glm::value_ptr(k_specular));
	glUniform3fv(glGetUniformLocation(shader, "k_ambient"), 1, glm::value_ptr(k_ambient));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);
	glUniform1i(glGetUniformLocation(shader, "isLightSource"), 1);

	// Send uniform light source values
	glUniform3fv(glGetUniformLocation(shader, "lightPosition"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));

	// Pass in which render mode we are in (normal, Phong)
	glUniform1i(glGetUniformLocation(shader, "render_mode"), renderMode);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	// glPointSize(pointSize);

	// Draw the points 
	// glDrawArrays(GL_POINTS, 0, points.size());
	// Draw the points using triangles, indexed with the EBO
	// glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

glm::vec3 LightSource::lerp(glm::vec3 start, glm::vec3 end, float t) {
	return start * (1.0f - t) + end * t;
}

void LightSource::update()
{
	// Spin the cube by 1 degree
	// spin(0.1f);
}

void LightSource::updateModelSize(double yoffset) {
	/*
	Scaling matrix form:
	[
		x 0 0 0
		0 y 0 0
		0 0 z 0
		0 0 0 1
	]
	Model matrix is already set in the vertex shader. Every frame (every call to draw),
	the uniform variable "model" is set. To do transformations (rotations, scale, etc.),
	we can concatenate the transformations by multiplying them with this model matrix
	*/
	if (yoffset > 0) {
		// Scale the model up by multiplying a scale matrix
		model = model * glm::scale(glm::vec3(1.1f, 1.1f, 1.1f));
	}
	else if (yoffset < 0) {
		// Scale the model up by multiplying a scale matrix
		model = model * glm::scale(glm::vec3(0.9f, 0.9f, 0.9f));
	}
}

void LightSource::initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	// Turn on user interactive rotations
	movement = 0;

	// Map the mouse position to a logical sphere location and keep track of the last known mouse position
	lastCursorPos = trackBallMapping(windowWidth, windowHeight, cursorPos);

	// Set the OpenGL state to modify the MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void LightSource::rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos) {
	glm::vec3 currSpherePos, rotateDirection;
	float rot_angle;

	// Check which type of movement we are doing
	switch (movement) {
		// If the movement is ROTATE
		case 0: {
			// Map this mouse position to it's logical sphere location
			currSpherePos = trackBallMapping(windowWidth, windowHeight, currCursorPos);

			// Determine the direction that the object should be rotated in logical sphere
			rotateDirection = currSpherePos - lastCursorPos;
			float velocity = glm::length(rotateDirection);

			// Only rotate if there is a decent amount of movement
			if (velocity > 0.0001f) {
				// Rotate about the axis that is perpendicular to the two points
				glm::vec3 rotAxis = glm::cross(lastCursorPos, currSpherePos);
				rot_angle = velocity * 40;

				// Calculate the rotation matrix
				model = glm::rotate(glm::radians(rot_angle), rotAxis) * model;

				// Rotate the actual light source position vector by converting the vector into world
				// coordinates by multiplying it by model, then applying the same rotation matrix
				// to the world coord light pos as to the actual model
				glm::vec4 lightPosWorld = model * glm::vec4(lightPosition, 1.0);
				lightPosition = glm::vec3(glm::rotate(glm::radians(rot_angle), rotAxis) * lightPosWorld);
			}

			// Save the current point location for the next movement
			lastCursorPos = currSpherePos;
			break;
		}
	}
}

void LightSource::endRotateModel() {
	// Turn off rotation
	movement = -1;
}

// Calculate 3D position of a projected unit vector onto the xy-plane
glm::vec3 LightSource::trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * cursorPos.x - windowWidth) / windowWidth;
	v.y = (windowHeight - 2.0 * cursorPos.y) / windowHeight;
	v.z = 0.0f;
	d = glm::length(v);
	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrtf(1.001f - d * d);
	v = glm::normalize(v);
	return v;
}

void LightSource::updateLightPositionToCenter(double yoffset) {
	// Transformations: translate to (0,0,0) -> scale wrt (0,0,0) -> inverse translate back
	glm::mat4 translateToOrigin = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 invTranslateToOrigin = glm::inverse(translateToOrigin);
	glm::mat4 scaleMatrix;

	if (yoffset > 0) {
		// Scale the model up by multiplying a scale matrix
		scaleMatrix = glm::scale(glm::vec3(1.1f, 1.1f, 1.1f));
	}
	else if (yoffset < 0) {
		// Scale the model up by multiplying a scale matrix
		scaleMatrix = glm::scale(glm::vec3(0.9f, 0.9f, 0.9f));
	}

	// Convert lightPos to world coordinates and multiply by the scale matrix to move the light position farther/closer to origin
	glm::vec4 lightPosWorld = model * glm::vec4(lightPosition, 1.0f);
	lightPosition = scaleMatrix * lightPosWorld;
	// Scale the model to get closer and farther from the origin
	model = scaleMatrix * model;
}

void LightSource::updatePointSize(GLfloat size)
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

void LightSource::setModelMaterialProperties(glm::vec3 k_d, glm::vec3 k_s, glm::vec3 k_a, float s) {
	// Set the uniform variable for k for diffuse, specular, ambient for this specific object
	k_diffuse = k_d;
	k_specular = k_s;
	k_ambient = glm::vec3(1.0f, 1.0f, 1.0f);//lightColor;
	shininess = s;
}

void LightSource::changeRenderingMode() {
	if (renderMode == 0) {
		renderMode = 1;
	}
	else {
		renderMode = 0;
	}
}

void LightSource::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
