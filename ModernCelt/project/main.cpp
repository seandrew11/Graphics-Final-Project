#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Terrain.h"
#include "render/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow* window;

static float viewAzimuth = 0.f;
static float viewPolar = 0.f;
static float viewDistance = 30.0f;
static glm::vec3 eye_center;
static glm::vec3 lookat(0, 0, 0);
static glm::vec3 up(0, 1, 0);

GLuint LoadTerrainTexture(const char* texture_file_path) {
    int w, h, channels;
    uint8_t* img = stbi_load(texture_file_path, &w, &h, &channels, 3);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (img) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(img);
    return texture;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Terrain Generation", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL functions
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, key_callback);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.25f, 0.0f);

    // Load shaders
    GLuint shaderProgram = LoadShadersFromFile("../project/terrain.vert", "../project/terrain.frag");
    if (shaderProgram == 0) {
        std::cerr << "Failed to load shaders" << std::endl;
        return -1;
    }


    // Create terrain
    Terrain terrain(50, 50);
    GLuint terrainTexture = LoadTerrainTexture("../project/textures/Grass_01.png");
    GLuint textureSamplerID = glGetUniformLocation(shaderProgram, "terrainTexture");
    terrain.setTexture(terrainTexture, textureSamplerID);

    // Camera setup
    eye_center = glm::vec3(viewDistance * cos(viewAzimuth), viewDistance * cos(viewPolar),
                          viewDistance * sin(viewAzimuth));

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 1000.0f);
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader and set MVP matrix
        glm::mat4 viewMatrix = glm::lookAt(eye_center, lookat, up);
        glm::mat4 MVP = projectionMatrix * viewMatrix;
        glUseProgram(shaderProgram);


        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Render terrain
        terrain.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (!glfwWindowShouldClose(window));

    // Cleanup
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
