#version 330 core
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec3 color;    // Vertex color

out vec3 vertexColor; // Pass color to the fragment shader

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    vertexColor = color;
}
