#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

out vec3 color;
out vec2 UV;
out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 MVP;
uniform mat4 model;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
    worldPosition = (model * vec4(vertexPosition, 1.0)).xyz;
    worldNormal = mat3(transpose(inverse(model))) * vertexNormal;
    color = vertexColor;
    UV = vertexUV;
}
