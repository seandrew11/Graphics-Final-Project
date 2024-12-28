#include "Terrain.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../project/include/PerlinNoise.hpp"

Terrain::Terrain(int w, int h, GLuint shader, glm::vec3 pos = glm::vec3(0.0f))
    : width(w),
      height(h),
      position(pos),
      VAO(0),
      VBO(0),
      EBO(0),
      textureID(0),
      modelMatrix(1.0f) {
    shaderProgram = shader;
    perlin.reseed(1234); // Initialize Perlin noise generator
    generateTerrain();   // Create terrain vertices and indices
    setupBuffers();      // Set up OpenGL buffers
}

Terrain::~Terrain() {
    cleanup(); // Free OpenGL resources
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
    return height * 12.0 / maxValue; // Scale height to match visual requirements
}

glm::vec3 Terrain::calculateNormal(int x, int z) {
    glm::vec3 p0(x - 1, getHeight(x - 1, z), z);
    glm::vec3 p1(x + 1, getHeight(x + 1, z), z);
    glm::vec3 p2(x, getHeight(x, z - 1), z - 1);
    glm::vec3 p3(x, getHeight(x, z + 1), z + 1);

    glm::vec3 v1 = p1 - p0;
    glm::vec3 v2 = p3 - p2;
    return glm::normalize(glm::cross(v2, v1));
}

void Terrain::setTexture(GLuint texID, GLuint samplerID) {
    textureID = texID;
    textureSamplerID = samplerID;
}

void Terrain::updateTerrain(glm::vec3 cameraPos) {
    float threshold = width / 2.0f; // Threshold for terrain updates

    float deltaX = cameraPos.x - offset.x;
    float deltaZ = cameraPos.z - offset.z;

    if (abs(deltaX) > threshold || abs(deltaZ) > threshold) {
        offset.x += round(deltaX / width) * width;
        offset.z += round(deltaZ / height) * height;

        for (auto& vertex : vertices) {
            vertex.position.x += round(deltaX / width) * width;
            vertex.position.z += round(deltaZ / height) * height;
            vertex.position.y = getHeight(vertex.position.x, vertex.position.z);
            vertex.normal = calculateNormal(vertex.position.x, vertex.position.z);
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Terrain::generateTerrain() {
    vertices.clear();
    indices.clear();

    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            Vertex vertex;
            float xPos = (float)x - width / 2.0f;
            float zPos = (float)z - height / 2.0f;
            float heightValue = getHeight(x, z);

            vertex.position = glm::vec3(xPos, heightValue, zPos);
            vertex.normal = calculateNormal(x, z);
            vertex.texCoord = glm::vec2(x / (float)width * 20.0f, z / (float)height * 20.0f);
            vertices.push_back(vertex);
        }
    }

    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int topLeft = z * width + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * width + x;
            unsigned int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

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

    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
}

void Terrain::render(const glm::mat4& mvpMatrix, const glm::vec3& lightPos, const glm::vec3& lightInt, const glm::mat4& lightSpaceMatrix) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform3fv(lightPositionID, 1, &lightPos[0]);
    glUniform3fv(lightIntensityID, 1, &lightInt[0]);
    glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapID);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 1);

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

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glUniformMatrix4fv(depthModelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(depthLightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

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
