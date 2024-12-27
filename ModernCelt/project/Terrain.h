#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "../project/include/PerlinNoise.hpp"
using namespace siv;

struct Vertex {
    glm::vec3 position;    // Position in 3D space
    glm::vec3 normal;      // Normal vector for lighting
    glm::vec2 texCoord;    // Texture coordinates
};

class Terrain {
public:
    Terrain(int width, int height, GLuint shader, glm::vec3 pos);
    ~Terrain();

    void renderDepth(const glm::mat4& lightSpaceMatrix);

    // Update existing render method signature
    void render(const glm::mat4& mvpMatrix, const glm::vec3& lightPos, const glm::vec3& lightInt, const glm::mat4& lightSpaceMatrix = glm::mat4(1.0f));
    void setTexture(GLuint texID, GLuint samplerID);
    void updateTerrain(glm::vec3 cameraPos);
    void cleanup();
    float getHeight(int x, int z);
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 offset = glm::vec3(0.0f);



private:
    GLuint textureID;
    GLuint textureSamplerID;
    siv::PerlinNoise perlin;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint modelMatrixID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint shaderProgram;
    GLuint mvpMatrixID;
    GLuint lightSpaceMatrixID;
    GLuint shadowMapID;
    GLuint depthModelID;
    GLuint depthLightSpaceMatrixID;
    glm::mat4 modelMatrix;


    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int width;
    int height;

    void generateTerrain();
    void setupBuffers();

    // Add these two function declarations
    glm::vec3 calculateNormal(int x, int z);
};
