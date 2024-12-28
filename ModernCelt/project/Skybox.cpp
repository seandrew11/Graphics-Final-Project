#include "Skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <render/shader.h>
#include <iostream>

// Vertex buffer data
const GLfloat Skybox::vertex_buffer_data[72] = {
    // Front face
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    // Back face
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    // Left face
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    // Right face
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,

    // Top face
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    // Bottom face
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
};

// Color buffer data
const GLfloat Skybox::color_buffer_data[72] = {
    // Face colors
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
};

// Index buffer data
const GLuint Skybox::index_buffer_data[36] = {
    0, 3, 2,
    0, 2, 1,

    4, 7, 6,
    4, 6, 5,

    8, 11, 10,
    8, 10, 9,

    12, 15, 14,
    12, 14, 13,

    16, 19, 18,
    16, 18, 17,

    20, 23, 22,
    20, 22, 21,
};

// UV buffer data
const GLfloat Skybox::uv_buffer_data[48] = {

    // Front face (Positive Z)
    0.5f, 0.666f,  // Top right
    0.25f, 0.666f, // Top left
    0.25f, 0.333f, // Bottom left
    0.5f, 0.333f,  // Bottom right

    // Back face (Negative Z)
    1.0f, 0.666f,  // Top right
    0.75f, 0.666f, // Top left
    0.75f, 0.333f, // Bottom left
    1.0f, 0.333f,  // Bottom right

    // Right face (Positive X)

    0.75f, 0.666f, // Top right
    0.5f, 0.666f,  // Top left
    0.5f, 0.333f,  // Bottom left
    0.75f, 0.333f, // Bottom right

    // Left face (Negative X)
    0.25f, 0.666f, // Top right
    0.0f, 0.666f,  // Top left
    0.0f, 0.333f,  // Bottom left
    0.25f, 0.333f, // Bottom right

    // Bottom face (Negative Y)

    0.5f, 0.333f,  // Top right
    0.25f, 0.333f,  // Top left
    0.25f, 0.0f,   // Bottom left
    0.5f, 0.0f,    // Bottom right

    0.5f, 1.0f,    // Top right
    0.25f, 1.0f,   // Top left
    0.25f, 0.666f, // Bottom left
    0.5f, 0.666f  // Bottom right
};

// Load skybox texture
GLuint LoadSkyBoxTexture(const char* texture_file_path) {
    int w, h, channels;
    uint8_t* img = stbi_load(texture_file_path, &w, &h, &channels, 3);
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (img) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texture_file_path << std::endl;
    }

    stbi_image_free(img);
    return texture;
}

// Initialize skybox
void Skybox::initialize(glm::vec3 position, glm::vec3 scale) {
    this->position = position;
    this->scale = scale;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &uvBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

    programID = LoadShadersFromFile("../project/skybox.vert", "../project/skybox.frag");
    if (programID == 0) {
        std::cerr << "Failed to load shaders." << std::endl;
    }

    mvpMatrixID = glGetUniformLocation(programID, "MVP");
    textureSamplerID = glGetUniformLocation(programID, "textureSampler");
    textureID = LoadSkyBoxTexture("../project/textures/sky.png");
}

// Render skybox
void Skybox::render(glm::mat4 cameraMatrix) {
    glUseProgram(programID);
    glBindVertexArray(vertexArrayID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, scale);
    glm::mat4 mvp = cameraMatrix * modelMatrix;

    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerID, 0);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindVertexArray(0);
}

// Cleanup skybox
void Skybox::cleanup() {
    if (vertexArrayID) glDeleteVertexArrays(1, &vertexArrayID);
    if (vertexBufferID) glDeleteBuffers(1, &vertexBufferID);
    if (colorBufferID) glDeleteBuffers(1, &colorBufferID);
    if (uvBufferID) glDeleteBuffers(1, &uvBufferID);
    if (indexBufferID) glDeleteBuffers(1, &indexBufferID);
    if (programID) glDeleteProgram(programID);
}
