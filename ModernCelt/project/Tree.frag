#version 330 core
in vec3 vertexColor;  // Interpolated color from the vertex shader
out vec4 fragColor;   // Final output color

void main() {
    fragColor = vec4(vertexColor, 1.0); // Use the vertex color
}
