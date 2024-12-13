#version 330 core

// Input from vertex shader
in vec2 TexCoord;
in vec3 Normal;

// Texture sampler
uniform sampler2D terrainTexture;

out vec4 FragColor;

void main() {
    vec4 textureColor = texture(terrainTexture, TexCoord);
    FragColor = textureColor;
}
