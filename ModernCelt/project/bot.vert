#version 330 core

// Attributes
layout(location = 0) in vec3 inPosition;   // Vertex position
layout(location = 1) in vec3 inNormal;     // Vertex normal
layout(location = 2) in vec2 inTexCoord;   // Texture coordinates
layout(location = 3) in uvec4 inJoints;    // Joint indices
layout(location = 4) in vec4 inWeights;    // Joint weights

// Uniforms
uniform mat4 MVP;                  // Model-View-Projection matrix
uniform mat4 jointMatrices[50];    // Array of joint matrices

// Outputs to the fragment shader
out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoord;  // Add this for texture coordinates

void main() {
    // Skinning transformation
    vec4 skinnedPosition = vec4(0.0);
    vec3 skinnedNormal = vec3(0.0);

    for (int i = 0; i < 4; i++) {
        float weight = inWeights[i];
        if (weight > 0.0) {
            uint jointIndex = inJoints[i];
            mat4 jointMatrix = jointMatrices[jointIndex];
            skinnedPosition += weight * (jointMatrix * vec4(inPosition, 1.0));
            mat3 jointMatrix3 = mat3(jointMatrix);
            skinnedNormal += weight * (jointMatrix3 * inNormal);
        }
    }

    // Pass data to fragment shader
    worldPosition = vec3(skinnedPosition);
    worldNormal = normalize(skinnedNormal);
    texCoord = inTexCoord;  // Pass texture coordinates

    gl_Position = MVP * skinnedPosition;
}