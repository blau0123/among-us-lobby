#version 330 core

in vec3 fragParticleColor;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(fragParticleColor, 1.0);
}
