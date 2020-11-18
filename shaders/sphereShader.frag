#version 330 core
// Note: shader.vert/frag is used for normal coloring/Phong illumination shading
// sphereShader.vert/frag is used for environment map reflections on the sphere to reflect the skybox around it
// skyBoxShader.vert/frag is used for the sky box texture
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
	//vec3 viewDirI = normalize(Position - cameraPos);
	//vec3 reflectDirR = reflect(viewDirI, normalize(Normal));
	//FragColor = vec4(texture(skybox, reflectDirR).rgb, 1.0);
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}