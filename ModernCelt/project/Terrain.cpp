#include "Terrain.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../project/include/PerlinNoise.hpp"

Terrain::Terrain(int w, int h, GLuint shader) : width(w),
    height(h),
    VAO(0),
    VBO(0),
    EBO(0),
    textureID(0),
    modelMatrix(1.0f){
    shaderProgram = shader;
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

    modelMatrixID = glGetUniformLocation(shaderProgram, "model");
    lightPositionID = glGetUniformLocation(shaderProgram, "lightPosition");
    lightIntensityID = glGetUniformLocation(shaderProgram, "lightIntensity");
    mvpMatrixID = glGetUniformLocation(shaderProgram, "MVP");
    lightSpaceMatrixID = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
    shadowMapID = glGetUniformLocation(shaderProgram, "shadowMap");
    depthModelID = glGetUniformLocation(shaderProgram, "model");
    depthLightSpaceMatrixID = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");

    if (modelMatrixID == -1 || lightPositionID == -1 ||
       lightIntensityID == -1 || mvpMatrixID == -1) {
        std::cerr << "Failed to get uniform locations in Terrain" << std::endl;
        std::cerr << "model: " << modelMatrixID << std::endl;
        std::cerr << "lightPos: " << lightPositionID << std::endl;
        std::cerr << "lightInt: " << lightIntensityID << std::endl;
        std::cerr << "MVP: " << mvpMatrixID << std::endl;
    }


    // Set up model matrix
    modelMatrix = glm::mat4(1.0f); // Identity matrix for terrain
}

void Terrain::render(const glm::mat4& mvpMatrix, const glm::vec3& lightPos, const glm::vec3& lightInt, const glm::mat4& lightSpaceMatrix) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Set existing uniforms
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform3fv(lightPositionID, 1, &lightPos[0]);
    glUniform3fv(lightIntensityID, 1, &lightInt[0]);

    // Add shadow mapping uniforms
    glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Bind shadow map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapID);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 1);

    // Bind terrain texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerID, 0);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Terrain::renderDepth(const glm::mat4& lightSpaceMatrix) {
    glBindVertexArray(VAO);

    // Only need position for depth pass
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Set uniforms for depth shader
    glUniformMatrix4fv(depthModelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(depthLightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Draw terrain
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
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
