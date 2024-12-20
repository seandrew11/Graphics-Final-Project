#include "Building.h"
#include "render/shader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Building::Building() : vertexArrayID(0), vertexBufferID(0), indexBufferID(0), colorBufferID(0), programID(0) {}

Building::~Building() {
    cleanup();
}

void Building::initialize(glm::vec3 position, glm::vec3 scale) {
    this->position = position;
    this->scale = scale;

   
    GLfloat vertex_buffer_data[] = {
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

    GLfloat color_buffer_data[72];

    for (int i = 0; i < 72; i += 3) {
        color_buffer_data[i] = 1.0f;    // R
        color_buffer_data[i+1] = 0.0f;  // G
        color_buffer_data[i+2] = 0.0f;  // B
    }

    // Create and bind VAO first
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create and bind vertex buffer
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    // Create and bind color buffer
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

    // Define index buffer data
    GLuint index_buffer_data[] = {
        // Front face
        0, 1, 2,
        0, 2, 3,
        // Back face
        4, 5, 6,
        4, 6, 7,
        // Left face
        8, 9, 10,
        8, 10, 11,
        // Right face
        12, 13, 14,
        12, 14, 15,
        // Top face
        16, 17, 18,
        16, 18, 19,
        // Bottom face
        20, 21, 22,
        20, 22, 23
    };

    // Create and bind index buffer
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

    // Load shaders
    programID = LoadShadersFromFile("../project/box.vert", "../project/box.frag");
    if (programID == 0) {
        std::cerr << "Failed to load building shaders." << std::endl;
        return;
    }

    // Get uniform location
    mvpMatrixID = glGetUniformLocation(programID, "MVP");
    if (mvpMatrixID == -1) {
        std::cerr << "Could not find MVP uniform in shader program" << std::endl;
    }
    std::cout << "MVP uniform location: " << mvpMatrixID << std::endl;

    // Set up vertex attributes while VAO is bound
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Unbind VAO
    glBindVertexArray(0);
}

void Building::render(glm::mat4 cameraMatrix) {
    glUseProgram(programID);
    glBindVertexArray(vertexArrayID);

    // Create model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);

    // Set MVP matrix
    glm::mat4 mvp = cameraMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

    // Draw building
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);
}

void Building::cleanup() {
    if (vertexArrayID != 0) {
        glDeleteVertexArrays(1, &vertexArrayID);
    }
    if (vertexBufferID != 0) {
        glDeleteBuffers(1, &vertexBufferID);
    }
    if (colorBufferID != 0) {
        glDeleteBuffers(1, &colorBufferID);
    }
    if (indexBufferID != 0) {
        glDeleteBuffers(1, &indexBufferID);
    }
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}
