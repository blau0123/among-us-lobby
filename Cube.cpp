#include "Cube.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cube::Cube(unsigned int *cubeMapID) 
{
	// Model matrix. Since the original size of the cube is 2, in order to
	// have a cube of some size, we need to scale the cube by size / 2.
	// model = glm::scale(glm::vec3(size / 2.f)); 
	model = glm::mat4(1);

	// The color of the cube. Try setting it to something else!
	color = glm::vec3(1.0f, 0.95f, 0.1f); 

	/*
	 * Cube indices used below.
	 *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
	 *
	 */

	// The 8 vertices of a cube.
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-500, 500, 500),
		glm::vec3(-500, -500, 500),
		glm::vec3(500, -500, 500),
		glm::vec3(500, 500, 500),
		glm::vec3(-500, 500, -500),
		glm::vec3(-500, -500, -500),
		glm::vec3(500, -500, -500),
		glm::vec3(500, 500, -500)
	}; 

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
	}; 

	// Load cubemap with the order: right -> left -> top -> botom -> front -> back
	std::vector<std::string> faces
	{
		"skybox_textures/Skybox_Water222_right.jpg",
		"skybox_textures/Skybox_Water222_left.jpg",
		"skybox_textures/Skybox_Water222_top.jpg",
		"skybox_textures/Skybox_Water222_base.jpg",
		"skybox_textures/Skybox_Water222_front.jpg",
		"skybox_textures/Skybox_Water222_back.jpg"
	};
	cubemapTextureID = loadCubemap(faces);
	*cubeMapID = cubemapTextureID;
	std::cout << "Loaded cube map with texture id: " << cubemapTextureID << std::endl;

	// Initialize model view matrix
	model = glm::mat4(1);

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

/*
Method for loading the 6 faces of the cubemap to render a skybox using this Cube class as the cubemap
*/
unsigned int Cube::loadCubemap(std::vector<std::string> faces) {
	// Bind the given texture id (will populate id)
	unsigned int textureID;
	// Returns unique texture name into textureID
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Note: nrChannels = number of color channels
	int width, height, nrChannels;
	// Loop through each of the 6 faces (paths to the face texture image), load the image,
	// then call glTextImage2D for each face
	for (unsigned int i = 0; i < faces.size(); i++) {
		// Note: data[0] = first px's R, data[1] = first px's G, data[2] = first px's B, data[3] = 2nd px's R, etc.
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			// Loads image into OpenGL texture in GPU memory
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::string fail_reason = "";
			if (stbi_failure_reason())
				fail_reason = stbi_failure_reason();
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << " due to reason: " << fail_reason << std::endl;
			stbi_image_free(data);
		}
	}

	// Cubemap = a texture, so we need to specify its wrapping and filtering methods
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Use billinear interpolation
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Use clamp to edge to hide skybox edges
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Cube::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Disable depth writing so that skybox always drawn at background of all other objects
	glDepthMask(GL_FALSE);
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader, "skybox"), 0);
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// 36 because will draw 36 elements, since it is a fixed cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Cube::update()
{
	// Spin the cube by 1 degree.
	// spin(0.1f);
}

void Cube::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
