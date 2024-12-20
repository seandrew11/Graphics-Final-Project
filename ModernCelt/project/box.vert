#version 330 core

// Input
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV; // Add UV input to the vertex shader

// Output data, to be interpolated for each fragment
out vec3 color;
out vec2 UV; // Output UV to the fragment shader

// Matrix for vertex transformation
uniform mat4 MVP;

void main() {
    // Transform vertex
    gl_Position = MVP * vec4(vertexPosition, 1);

    // Pass vertex color to the fragment shader
    color = vertexColor;

    // Pass UV to the fragment shader
    UV = vertexUV; // Assign the input UV to the output UV
}
