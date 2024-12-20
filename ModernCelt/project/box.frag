// box.frag
#version 330 core

in vec3 fragmentColor;
out vec4 color;  // Changed to vec4

void main() {
    color = vec4(fragmentColor, 1.0);  // Added alpha channel
}