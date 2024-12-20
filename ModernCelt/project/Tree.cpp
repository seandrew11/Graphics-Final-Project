#include "Tree.h"
#include "render/shader.h"


// Trunk vertices
const GLfloat Tree::trunkVertices[72] = {
    // Trunk base
    -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, -0.5f,  0.5f, 1.0f, -0.5f,  -0.5f, 1.0f, -0.5f, // Front
    -0.5f, 0.0f,  0.5f,  0.5f, 0.0f,  0.5f,  0.5f, 1.0f,  0.5f,  -0.5f, 1.0f,  0.5f, // Back
    -0.5f, 0.0f, -0.5f, -0.5f, 0.0f,  0.5f, -0.5f, 1.0f,  0.5f,  -0.5f, 1.0f, -0.5f, // Left
     0.5f, 0.0f, -0.5f,  0.5f, 0.0f,  0.5f,  0.5f, 1.0f,  0.5f,   0.5f, 1.0f, -0.5f, // Right
    -0.5f, 1.0f, -0.5f,  0.5f, 1.0f, -0.5f,  0.5f, 1.0f,  0.5f,  -0.5f, 1.0f,  0.5f, // Top
    -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, -0.5f,  0.5f, 0.0f,  0.5f,  -0.5f, 0.0f,  0.5f  // Bottom
};

const GLuint Tree::trunkIndices[36] = {
    0, 1, 2,  0, 2, 3,   // Front
    4, 5, 6,  4, 6, 7,   // Back
    8, 9, 10, 8, 10, 11, // Left
    12, 13, 14, 12, 14, 15, // Right
    16, 17, 18, 16, 18, 19, // Top
    20, 21, 22, 20, 22, 23  // Bottom
};

const GLfloat foliageVertices[72] = {
    // Front
    -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
    // Back
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
    // Left
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, -0.5f,
    // Right
    0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
    // Top
    -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, -0.5f,  -0.5f, 0.5f, -0.5f,
    // Bottom
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f
};

const GLuint foliageIndices[36] = {
    0, 1, 2,  0, 2, 3,   // Front
    4, 5, 6,  4, 6, 7,   // Back
    8, 9, 10, 8, 10, 11, // Left
    12, 13, 14, 12, 14, 15, // Right
    16, 17, 18, 16, 18, 19, // Top
    20, 21, 22, 20, 22, 23  // Bottom
};

Tree::Tree() : trunkVAO(0), trunkVBO(0), trunkEBO(0), foliageVAO(0), foliageVBO(0), foliageColorBufferID(0), trunkColorBufferID(0), foliageIndexBufferID(0) {}

Tree::~Tree() {
    cleanup();
}

void Tree::initialize(glm::vec3 position, glm::vec3 trunkScale, float foliageRadius) {
    this->position = position;
    this->trunkScale = trunkScale;
    this->foliageRadius = foliageRadius;
    this->foliagePosition = glm::vec3(0.0f, trunkScale.y, 0.0f);

    shaderProgram = LoadShadersFromFile("../project/tree.vert", "../project/tree.frag");
    mvpMatrixID = glGetUniformLocation(shaderProgram, "MVP");

    // Initialize the trunk with brown color
    GLfloat trunkColors[72];
    for (int i = 0; i < 72; i += 3) {
        trunkColors[i] = 0.55f;    // R
        trunkColors[i + 1] = 0.27f; // G
        trunkColors[i + 2] = 0.07f; // B
    }

    glGenBuffers(1, &trunkColorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, trunkColorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trunkColors), trunkColors, GL_STATIC_DRAW);

    // Trunk VAO/VBO setup
    glGenVertexArrays(1, &trunkVAO);
    glGenBuffers(1, &trunkVBO);
    glGenBuffers(1, &trunkEBO);

    glBindVertexArray(trunkVAO);

    glBindBuffer(GL_ARRAY_BUFFER, trunkVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trunkVertices), trunkVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trunkEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trunkIndices), trunkIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, trunkColorBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Initialize foliage with green color
    GLfloat foliageColors[72];
    for (int i = 0; i < 72; i += 3) {
        foliageColors[i] = 0.0f;  // Red
        foliageColors[i + 1] = 0.4f;  // Green
        foliageColors[i + 2] = 0.0f;  // Blue
    }

    glGenVertexArrays(1, &foliageVAO);
    glGenBuffers(1, &foliageVBO);
    glGenBuffers(1, &foliageColorBufferID);
    glGenBuffers(1, &foliageIndexBufferID);

    glBindVertexArray(foliageVAO);

    // Foliage vertices
    glBindBuffer(GL_ARRAY_BUFFER, foliageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(foliageVertices), foliageVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Foliage colors
    glBindBuffer(GL_ARRAY_BUFFER, foliageColorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(foliageColors), foliageColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Foliage indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, foliageIndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(foliageIndices), foliageIndices, GL_STATIC_DRAW);
}

void Tree::render(glm::mat4 cameraMatrix) {
    glUseProgram(shaderProgram);

    // Render trunk
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, trunkScale);
    glm::mat4 mvp = cameraMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(trunkVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Render foliage
    modelMatrix = glm::translate(glm::mat4(1.0f), position + foliagePosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(foliageRadius)); // Use foliageRadius as the scale
    mvp = cameraMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(foliageVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Tree::cleanup() {
    glDeleteVertexArrays(1, &trunkVAO);
    glDeleteBuffers(1, &trunkVBO);
    glDeleteBuffers(1, &trunkEBO);
    glDeleteVertexArrays(1, &foliageVAO);
    glDeleteBuffers(1, &foliageVBO);
    glDeleteBuffers(1, &foliageColorBufferID);
    glDeleteBuffers(1, &trunkColorBufferID);
    glDeleteBuffers(1, &foliageIndexBufferID);
}
