#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Building {
public:
    glm::vec3 position;      // Position of the box
    glm::vec3 scale;         // Size of the box in each axis

    Building();              // Constructor
    ~Building();             // Destructor

    void initialize(glm::vec3 position, glm::vec3 scale, GLuint textureID, glm::vec3 lightPos, glm::vec3 lightInt);
    void render(glm::mat4 cameraMatrix);
    void cleanup();
    static const GLfloat vertex_buffer_data[72];  // Vertex data for a box
    static const GLfloat color_buffer_data[72];   // Color data for each vertex
    static const GLuint index_buffer_data[36];    // Indices for the box faces
    static const GLfloat uv_buffer_data[48];      // UV texture coordinates
    static const GLfloat normal_buffer_data[60];
    glm::vec3 lightPosition;
    glm::vec3 lightIntensity;
    glm::mat4 modelMatrix;
    glm::mat4 lightSpaceMatrix;

    // OpenGL buffer and texture IDs
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint uvBufferID;
    GLuint normalBufferID;
    GLuint textureID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint modelID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint textureSamplerID;
    GLuint programID;

};

GLuint LoadTextureTileBox(const char *texture_file_path); // Helper function to load textures
