#pragma once

#include <glm/glm.hpp>

#include <glad/gl.h>



class Skybox {

public:

    glm::vec3 position;       // Position of the skybox
    glm::vec3 scale;          // Scale of the skybox

    // OpenGL buffers

    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint uvBufferID;
    GLuint textureID;

    // Shader variable IDs

    GLuint mvpMatrixID;
    GLuint textureSamplerID;
    GLuint programID;

    void initialize(glm::vec3 position, glm::vec3 scale);
    void render(glm::mat4 cameraMatrix);
    void cleanup();



private:

    // Vertex, color, UV, and index data

    static const GLfloat vertex_buffer_data[72];
    static const GLfloat color_buffer_data[72];
    static const GLuint index_buffer_data[36];
    static const GLfloat uv_buffer_data[48];

};



// Helper function for loading texture

GLuint LoadTextureTileBox(const char* texture_file_path);