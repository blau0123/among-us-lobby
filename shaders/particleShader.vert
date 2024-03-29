#version 330 core

layout (location = 0) in vec3 particlePos;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform vec3 cameraPos;
uniform vec3 particleColor;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform int pointSize;

const float minPointScale = 0.1f;
const float maxPointScale = 0.7f;
const float maxDistance = 100.0f;

out vec3 fragParticleColor;

void main()
{
	// Calculate point scale based on distance from viewer
	float cameraDist = distance(particlePos, cameraPos);
	float pointScale = 1.0 - (cameraDist / maxDistance);
	pointScale = max(pointScale, minPointScale);
	pointScale = min(pointScale, maxPointScale);

	// Set GL globals and forward the color
	gl_Position = projection * view * model * vec4(particlePos, 1.0);
	gl_PointSize = pointSize * pointScale;
	//gl_PointSize = pointSize;
	fragParticleColor = particleColor;
}