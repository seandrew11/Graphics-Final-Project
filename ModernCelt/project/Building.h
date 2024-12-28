#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Building {
public:
    glm::vec3 position;      // Position of the building
    glm::vec3 scale;         // Scale of the building

    Building();              // Constructor
    ~Building();             // Destructor

    void initialize(glm::vec3 position, glm::vec3 scale, GLuint textureID);
    void render(const glm::mat4& cameraMatrix, const glm::vec3& lightPos, const glm::vec3& lightInt, const glm::mat4& lightSpaceMatrix);
    void cleanup();
    void renderDepth(const glm::mat4& lightSpaceMatrix);

    // Static data for the building's geometry
    static const GLfloat vertex_buffer_data[72];
    static const GLfloat color_buffer_data[72];
    static const GLuint index_buffer_data[36];
    static const GLfloat uv_buffer_data[48];
    static const GLfloat normal_buffer_data[72];

    // OpenGL resource IDs
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
    GLuint lightSpaceMatrixID;
    GLuint shadowMapID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint textureSamplerID;
    GLuint programID;
    GLuint depthModelID;
    GLuint depthLightSpaceMatrixID;

    glm::mat4 modelMatrix;
    glm::mat4 lightSpaceMatrix;
};

GLuint LoadTextureTileBox(const char *texture_file_path);
