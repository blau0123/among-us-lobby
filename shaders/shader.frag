#version 330 core
// This is a sample fragment shader.
uniform vec3 lightPosition;
// Linear attenuation constant used to calculate attenuation for point lights
uniform float lightLinear;
uniform vec3 lightColor;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 color;
// Material properties for the specific object being rendered with this fragment shader
uniform vec3 k_diffuse;
uniform vec3 k_specular;
uniform vec3 k_ambient;
uniform int isLightSource;
uniform float shininess;

uniform int render_mode;

// Possibly = to eyePos in Window?
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Normalize the properties
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 resultColor = vec3(0.0, 0.0, 0.0);

    // If render mode = normal, calculate the color based on normals by shift norm range from -1..1 to 0..1
    // Normal's x = R, y = G, z = B
    if (render_mode == 0) {
        resultColor = (normal + 1) / 2;
    }
    else {
        // Calculate a single point light
        vec3 lightDir = normalize(lightPosition - fragPos);
        // Calculate diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);

        // Calculate specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

        // (Linear) attenuation
        float distance = length(lightPosition - fragPos);
        float attenuation = 1.0f / (lightLinear * distance);

        // Calculate ambient, diffuse, and specular
        vec3 ambient = lightColor * k_ambient;
        vec3 diffuse = lightColor * k_diffuse * diff;
        vec3 specular = lightColor * spec * k_specular;

        // Multiply by the attenuation
        // If a light source, don't multiply ambient by attentuation, so that the light source
        // color won't change as you move it
        if (isLightSource == 0) {
            ambient *= attenuation;
        }
        diffuse *= attenuation;
        specular *= attenuation;

        resultColor = ambient + diffuse + specular;// CalcPointLight(norm, fragPos, viewDir);
    }

    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(resultColor, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

/* Will use light properties defined in the uniform variables
vec3 CalcPointLight(vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(lightPosition - fragPos);
    // Calculate diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Calculate specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // (Linear) attenuation
    float distance = length(lightPosition - fragPos);
    float attenuation = 1.0f / (lightLinear * distance);

    // Calculate ambient, diffuse, and specular
    vec3 ambient = lightColor * k_ambient;
    vec3 diffuse = lightColor * k_diffuse * diffuse;
    vec3 specular = lightColor * spec * k_specular;

    // Multiply by the attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
*/