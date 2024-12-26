#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

out vec2 TexCoord;
out vec3 worldNormal;
out vec3 worldPosition;

uniform mat4 MVP;
uniform mat4 model;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);
    worldPosition = (model * vec4(vertexPosition, 1.0)).xyz;
    worldNormal = normalize(mat3(transpose(inverse(model))) * vertexNormal);
    TexCoord = vertexTexCoord;
}