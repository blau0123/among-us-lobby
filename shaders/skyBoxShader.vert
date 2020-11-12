#version 330 core
// Layout location holds the position of the specific vertex
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	// Set texture coordinates for this vertex to be the vertex position
	TexCoords = aPos;

	// Built-in GLSL variable that contains the transformed posit of the current vertex
	gl_Position = projection * view * vec4(aPos, 1.0);
}