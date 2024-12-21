#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texCoord;

out vec3 finalColor;

// Light uniforms
uniform vec3 lightPosition;
uniform vec3 lightIntensity;

// Texture uniforms
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;

void main()
{
    // Sample textures
    vec3 albedo = texture(diffuseMap, texCoord).rgb;
    float ao = texture(aoMap, texCoord).r;

    // Normal mapping (simplified for now)
    vec3 normal = normalize(worldNormal);

    // Lighting calculation
    vec3 lightDir = lightPosition - worldPosition;
    float lightDist = dot(lightDir, lightDir);
    lightDir = normalize(lightDir);

    // Combine lighting with textures
    vec3 v = lightIntensity * albedo * ao * clamp(dot(lightDir, normal), 0.0, 1.0) / lightDist;

    // Tone mapping
    v = v / (1.0 + v);

    // Gamma correction
    finalColor = pow(v, vec3(1.0 / 2.2));
}