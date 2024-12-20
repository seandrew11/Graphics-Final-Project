#version 330 core

// Input data from the vertex shader
in vec3 color;
in vec2 UV; // Add UV input to the fragment shader

// Texture sampler
uniform sampler2D textureSampler; // Add the texture sampler

// Output color
out vec3 finalColor;

void main()
{
    // Perform a texture lookup using the UV coordinates
    vec3 textureColor = texture(textureSampler, UV).rgb;

    // Combine the texture color with the vertex color
    finalColor = textureColor * color;
}
