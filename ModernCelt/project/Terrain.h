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
    Terrain(int width, int height, GLuint shader);
    ~Terrain();

    void render(const glm::mat4& mvpMatrix, const glm::vec3& lightPos, const glm::vec3& lightInt);
    void setTexture(GLuint texID, GLuint samplerID);
    void cleanup();
    float getHeight(int x, int z);


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
