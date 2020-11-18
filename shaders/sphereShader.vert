#version 330 core
// In Sphere::init(), set these locations to be vertices and normals with 	
// glVertexAttribPointer([layout location], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat) [because vec3 with 3 floats], 0);
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	// Convert normal and position vectors to world coordinates
	//Normal = mat3(transpose(inverse(model))) * aNormal;
	//Position = vec3(model * vec4(aPos, 1.0));
	// Calc the transformed position of this vertex in world coordinates
	gl_Position = projection * view * model * vec4(Position, 1.0);
}