// box.vert
#version 330 core

layout(location = 0) in vec3 position;  // Changed from vertexPosition
layout(location = 1) in vec3 color;    // Changed from vertexColor

uniform mat4 MVP;

out vec3 fragmentColor;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    fragmentColor = color;
}