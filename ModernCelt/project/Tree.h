#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Tree {
public:
    glm::vec3 position;       // Position of the tree
    glm::vec3 trunkScale;     // Scale of the trunk
    float foliageRadius;      // Radius of the foliage sphere

    Tree();                   // Constructor
    ~Tree();                  // Destructor

    void initialize(glm::vec3 position, glm::vec3 trunkScale, float foliageRadius);
    void render(glm::mat4 cameraMatrix);
    void cleanup();

private:
    GLuint trunkVAO, trunkVBO, trunkEBO;     // Buffers for trunk
    GLuint foliageVAO, foliageVBO;          // Buffers for foliage
    GLuint trunkColorBufferID;              // Color buffer for trunk
    GLuint foliageColorBufferID;            // Color buffer for foliage
    GLuint foliageIndexBufferID;            // Index buffer for foliage
    GLuint shaderProgram;                   // Shader program
    GLuint mvpMatrixID;                     // MVP uniform location

    glm::vec3 foliagePosition;              // Offset position for foliage

    int foliageIndexCount;                  // Number of indices for foliage

    static const GLfloat trunkVertices[72]; // Vertex data for the trunk
    static const GLuint trunkIndices[36];   // Index data for the trunk

    void generateSphere();                  // Generate sphere vertices and indices
};
