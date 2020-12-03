#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
	// Decide the fragment color by sampling the skybox texture at TexCoords
	FragColor = texture(skybox, TexCoords);
}