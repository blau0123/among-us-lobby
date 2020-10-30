#version 330 core
// This is a sample fragment shader.
uniform vec4 lightPosition;
uniform vec3 lightIntensity;
uniform float lightAttenuation;
uniform float lightAmbientCoefficient;
uniform float lightConeAngle;
uniform vec3 lightConeDirection;
uniform float lightExponent;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
// in vec3 fragVert;
// in vec3 fragNormal;

uniform vec3 color;
// Material properties for the specific object being rendered with this fragment shader
// uniform vec3 k_diffuse;
// uniform vec3 k_specular;
// uniform vec3 k_ambient;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(color, sampleExtraOutput);
}