#version 330 core
in vec3 color;
in vec2 UV;
in vec3 worldPosition;
in vec3 worldNormal;

uniform sampler2D textureSampler;
uniform vec3 lightPosition;
uniform vec3 lightIntensity;

out vec3 finalColor;

void main() {
    // Get base color from texture only
    vec3 baseColor = texture(textureSampler, UV).rgb;

    // Calculate lighting
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition);
    float distance = length(lightPosition - worldPosition);

    // Softer distance attenuation
    float attenuation = 1.0 / (1.0 + 0.01 * distance + 0.001 * distance * distance);

    // Diffuse lighting
    float lambertian = max(dot(N, L), 0.0);
    vec3 diffuse = lambertian * baseColor * lightIntensity * attenuation;

    // Increased ambient lighting for better base visibility
    vec3 ambient = 0.2 * baseColor;

    // Final color with exposure tone mapping
    vec3 combined = ambient + diffuse;
    finalColor = combined / (combined + vec3(1.0));
}