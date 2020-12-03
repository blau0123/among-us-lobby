#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>

Geometry::Geometry(std::string filename, int bfCull, int useTexture, int useToonShading) {
	name = filename;
	useTex = useTexture;
	useToon = useToonShading;
	requiresBackfaceCull = bfCull;
	init(filename);
}

Geometry::~Geometry()
{
	// Delete the VBO, EBO and the VAO.
	glDeleteBuffers(1, &vboVertex);
	glDeleteBuffers(1, &vboNormals);
	glDeleteBuffers(1, &eboIndices);
	glDeleteBuffers(1, &vbo_t);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::init(std::string filename) {
	std::ifstream objFile(filename);
	// Hold the indices for vertices and normals in order to get a vertices and normals vector with same ordering
	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;
	std::vector<int> texIndices;

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
			else if (label == "vt") {
				glm::vec2 tex;
				ss >> tex.x >> tex.y;
				texCoords.push_back(tex);
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

				// std::cout << "1st: " << pairsX.find("/", 0) + 1 << "; 2nd: " << pairsX.find("/", lastX) << std::endl;
				texIndices.push_back(std::stoi(pairsX.substr(pairsX.find("/", 0) + 1, pairsX.find("/", lastX))));
				texIndices.push_back(std::stoi(pairsY.substr(pairsY.find("/", 0) + 1, pairsY.find("/", lastY))));
				texIndices.push_back(std::stoi(pairsZ.substr(pairsZ.find("/", 0) + 1, pairsZ.find("/", lastZ))));

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

	std::cout << "Number of points read for object " << filename << ": " << vertices.size() << std::endl;
	std::cout << "Number of vertex norms read for object " << filename << ": " << vertexNorms.size() << std::endl;
	std::cout << "Number of faces read for object " << filename << ": " << indices.size() << std::endl;

	// For each vertex of each triangle, add the vertex position with it's corresponding normal
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the vertex and normal from the given index
		int vertexIndex = vertexIndices[i];
		int normIndex = normalIndices[i];
		int texIndex = texIndices[i];
		// std::cout << texIndex << std::endl;
		// std::cout << "About to access vertex index: " << vertexIndex << std::endl;
		glm::vec3 vertex = vertices[vertexIndex - 1];
		glm::vec3 normal = vertexNorms[normIndex - 1];
		glm::vec2 tex = texCoords[texIndex - 1];
		out_vertices.push_back(vertex);
		out_normals.push_back(normal);
		out_tex.push_back(tex);
	}

	for (unsigned int i = 0; i < vertexIndices.size() - 2; i += 3) {
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
	glGenBuffers(1, &vbo_t);

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

	// Bind vbo_t to the bound VAO, and store the texture data
	glBindBuffer(GL_ARRAY_BUFFER, vbo_t);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNorms.size(), vertexNorms.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* out_tex.size(), &out_tex[0], GL_STATIC_DRAW);

	// Enable Vertex Attribute 1 to pass vertex norm data to the shader
	glEnableVertexAttribArray(2);
	// Location for the vertex norm layout variable in the vertex shader
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &eboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)* out_indices.size(), &out_indices[0], GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "Finished initing Geometry obj " << name << std::endl;
}

GLuint Geometry::loadTexture(std::string texLocation) {
	GLuint texId;
	// Get unique ID for texture, and tell OpenGL which texture to edit
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture edge mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Note: nrChannels = number of color channels
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	// Note: data[0] = first px's R, data[1] = first px's G, data[2] = first px's B, data[3] = 2nd px's R, etc.
	unsigned char* data = stbi_load(texLocation.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		// Loads image into OpenGL texture in GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else {
		std::string fail_reason = "";
		if (stbi_failure_reason())
			fail_reason = stbi_failure_reason();
		std::cout << "Texture failed to load at path: " << texLocation << " due to reason: " << fail_reason << std::endl;
		stbi_image_free(data);
	}
	std::cout << "Successfully loaded texture " << texLocation << std::endl;
	textureId = texId;
	return texId;
}

void Geometry::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewDir, GLuint shader) {
	// Apply the parent's transformations to the Geometry's model so that it is in the correct position relative to the parent
	//model = C;
	glm::mat4 matToPass = C * model;
	// std::cout << name << ": " << glm::to_string(matToPass) << std::endl;

	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(shader, "viewDir"), 1, glm::value_ptr(viewDir));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(matToPass));
	
	glUniform3fv(glGetUniformLocation(shader, "k_diffuse"), 1, glm::value_ptr(k_diffuse));
	glUniform3fv(glGetUniformLocation(shader, "k_specular"), 1, glm::value_ptr(k_specular));
	glUniform3fv(glGetUniformLocation(shader, "k_ambient"), 1, glm::value_ptr(k_ambient));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);
	glUniform1i(glGetUniformLocation(shader, "isLightSource"), 0);
	glUniform1i(glGetUniformLocation(shader, "tex"), 0);
	glUniform1i(glGetUniformLocation(shader, "use_texture"), useTex);
	glUniform1i(glGetUniformLocation(shader, "use_toon"), useToon);

	// Pass in which render mode we are in (normal, Phong)
	glUniform1i(glGetUniformLocation(shader, "render_mode"), 1);
	glUniform1i(glGetUniformLocation(shader, "is_ground"), isGround);

	// Bind the VAO
	glBindVertexArray(vao);
	if (requiresBackfaceCull == 1) 
		glDisable(GL_CULL_FACE);
	if (useTex == 1)
		glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	if (requiresBackfaceCull == 1)
		glEnable(GL_CULL_FACE);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
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