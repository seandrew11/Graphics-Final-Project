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
    Terrain(int width, int height);
    ~Terrain();

    void render();
    void setTexture(GLuint texID, GLuint samplerID);

private:
    GLuint textureID;
    GLuint textureSamplerID;
    siv::PerlinNoise perlin;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int width;
    int height;

    void generateTerrain();
    void setupBuffers();
    void cleanup();

    // Add these two function declarations
    float getHeight(int x, int z);
    glm::vec3 calculateNormal(int x, int z);
};
