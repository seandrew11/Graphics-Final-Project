#version 330 core
in vec2 TexCoord;
in vec3 worldNormal;
in vec3 worldPosition;

uniform sampler2D terrainTexture;
uniform vec3 lightIntensity;
uniform vec3 lightPosition;

out vec4 FragColor;

void main() {
    vec3 baseColor = texture(terrainTexture, TexCoord).rgb;
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition);
    float distance = length(lightPosition - worldPosition);

    float attenuation = 1.0 / (1.0 + 0.01 * distance + 0.001 * distance * distance);
    float lambertian = max(dot(N, L), 0.0);
    vec3 diffuse = lambertian * baseColor * lightIntensity * attenuation;
    vec3 ambient = 0.2 * baseColor;

    vec3 combined = ambient + diffuse;
    vec3 final = combined / (combined + vec3(1.0));

    FragColor = vec4(final, 1.0);
}