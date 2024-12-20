#version 330 core

in vec2 UV;
uniform sampler2D textureSampler;
out vec4 color;

void main() {
    // Sample the texture and output as vec4
    color = texture(textureSampler, UV);
}
