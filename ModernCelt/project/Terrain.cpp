#include "Terrain.h"
#include <iostream>
#include "../project/include/PerlinNoise.hpp"

Terrain::Terrain(int w, int h) : width(w), height(h), VAO(0), VBO(0), EBO(0), textureID(0) {
    perlin.reseed(1234);
    generateTerrain();
    setupBuffers();
}

Terrain::~Terrain() {
    cleanup();
}

float Terrain::getHeight(int x, int z) {

    const double scale = 0.03;
    const int octaves = 4;
    const double persistence = 0.5;

    double amplitude = 18.0;
    double frequency = scale;
    double height = 0.0;
    double maxValue = 0.0;

    for (int i = 0; i < octaves; i++) {
        height += perlin.noise2D(x * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    return height * 12.0 / maxValue;
}

glm::vec3 Terrain::calculateNormal(int x, int z) {
    float heightL = getHeight(x-1, z);
    float heightR = getHeight(x+1, z);
    float heightD = getHeight(x, z-1);
    float heightU = getHeight(x, z+1);

    glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
    return glm::normalize(normal);
}

void Terrain::setTexture(GLuint texID, GLuint samplerID) {
    textureID = texID;
    textureSamplerID = samplerID;
}


void Terrain::generateTerrain() {
    // Generate grid vertices
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            Vertex vertex;
            float xPos = (float)x - width/2.0f;
            float zPos = (float)z - height/2.0f;

            // Calculate height using sine waves
            float heightValue = getHeight(x, z);

            vertex.position = glm::vec3(xPos, heightValue, zPos);
            vertex.normal = calculateNormal(x, z);
            vertex.texCoord = glm::vec2(x / (float)width * 20.0f, z / (float)height * 20.0f);
            vertices.push_back(vertex);
        }
    }

    // Generate indices for triangles
    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int topLeft = z * width + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * width + x;
            unsigned int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

void Terrain::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Terrain::render() {
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerID,0);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }
}
