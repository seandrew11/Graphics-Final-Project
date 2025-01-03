#pragma once

#include "Building.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class IrishPub : public Building {
public:
    IrishPub() : frontTextureID(0), sideTextureID(0) {}

    // Initialize the IrishPub object with position, scale, textures, and lighting properties
    void initialize(glm::vec3 position, glm::vec3 scale, GLuint frontTex, GLuint sideTex,
                glm::vec3 lightPos, glm::vec3 lightInt) {
        this->position = position;
        this->scale = scale;
        this->frontTextureID = frontTex;
        this->sideTextureID = sideTex;
        this->lightPosition = lightPos;
        this->lightIntensity = lightInt;

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        // Initialize vertex buffer
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        // Initialize color buffer with white color
        GLfloat temp_color_buffer[72];
        for (int i = 0; i < 72; i++) {
            temp_color_buffer[i] = 1.0f;
        }
        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(temp_color_buffer), temp_color_buffer, GL_STATIC_DRAW);

        // Initialize UV buffer with tiling on V coordinate
        GLfloat temp_uv_buffer[48];
        memcpy(temp_uv_buffer, uv_buffer_data, sizeof(uv_buffer_data));
        for (int i = 0; i < 24; ++i) {
            temp_uv_buffer[2 * i + 1] *= 5.0f;
        }
        glGenBuffers(1, &uvBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(temp_uv_buffer), temp_uv_buffer, GL_STATIC_DRAW);

        // Initialize index buffer
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        // Initialize normal buffer
        glGenBuffers(1, &normalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);

        // Load shaders and get uniform locations
        programID = LoadShadersFromFile("../project/box.vert", "../project/box.frag");
        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        textureSamplerID = glGetUniformLocation(programID, "textureSampler");
        lightPositionID = glGetUniformLocation(programID, "lightPosition");
        lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
        modelID = glGetUniformLocation(programID, "model");
        lightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
        shadowMapID = glGetUniformLocation(programID, "shadowMap");
        depthModelID = glGetUniformLocation(programID, "model");
        depthLightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
    }

    // Render the pub with textures and lighting
    void render(glm::mat4 cameraMatrix, const glm::mat4& lightSpaceMatrix = glm::mat4(1.0f)) {
        glUseProgram(programID);
        glBindVertexArray(vertexArrayID);

        // Enable and set vertex attributes
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

        // Set transformation matrices and lighting uniforms
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::scale(modelMatrix, scale);
        glm::mat4 mvp = cameraMatrix * modelMatrix;

        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3fv(lightPositionID, 1, &lightPosition[0]);
        glUniform3fv(lightIntensityID, 1, &lightIntensity[0]);

        // Bind shadow map texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowMapID);
        glUniform1i(glGetUniformLocation(programID, "shadowMap"), 1);

        // Render front face with front texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frontTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glUniform1i(textureSamplerID, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Render other faces with side texture
        glBindTexture(GL_TEXTURE_2D, sideTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

        // Disable attributes
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glBindVertexArray(0);
    }

    // Render the pub for depth pass (shadow mapping)
    void renderDepth(const glm::mat4& lightSpaceMatrix) {
        glBindVertexArray(vertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::scale(modelMatrix, scale);

        glUniformMatrix4fv(depthModelID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(depthLightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        // Draw all faces for shadow mapping
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }

private:
    GLuint frontTextureID;
    GLuint sideTextureID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint modelID;
    GLuint normalBufferID;
    glm::vec3 lightPosition;
    glm::vec3 lightIntensity;
    GLuint lightSpaceMatrixID;
    GLuint shadowMapID;
    GLuint depthModelID;
    GLuint depthLightSpaceMatrixID;

    using Building::textureID; // Hide base class textureID
};
