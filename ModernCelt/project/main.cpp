#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Building.h"
#include "Skybox.h"
#include "Terrain.h"
#include "render/shader.h"
#include "stb_image.h"

// Global Variables

GLFWwindow* window;

// View control variables
static float viewAzimuth = 0.f;
static float viewPolar = 0.f;
static float viewDistance = 300.0f;
static glm::vec3 eye_center;
static glm::vec3 lookat(0, 0, 0);
static glm::vec3 up(0, 1, 0);

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        viewAzimuth = 0.f;
        viewPolar = 0.f;
        eye_center.y = viewDistance * cos(viewPolar);
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        viewPolar -= 0.1f;
        eye_center.y = viewDistance * cos(viewPolar);
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        viewPolar += 0.1f;
        eye_center.y = viewDistance * cos(viewPolar);
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        viewAzimuth -= 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        viewAzimuth += 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Terrain and Buildings", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load OpenGL functions
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    GLuint shaderProgram = LoadShadersFromFile("../project/terrain.vert", "../project/terrain.frag");
    if (shaderProgram == 0) {
        std::cerr << "Failed to load shaders." << std::endl;
        return -1;
    }

    // Initialize objects
    Skybox skybox;
    skybox.initialize(glm::vec3(0.0f), glm::vec3(500.0f));

    Terrain terrain(500, 500);
    GLuint terrainTexture = LoadTextureTileBox("../project/textures/Grass_01.png");
    GLuint terrainSampler = glGetUniformLocation(shaderProgram, "terrainTexture");
    terrain.setTexture(terrainTexture, terrainSampler);

    GLuint buildingTexture = LoadTextureTileBox("../project/textures/facade2.jpg");
    Building building;
    building.initialize(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(5.0f, 40.0f, 5.0f), buildingTexture);

    // Camera setup
    eye_center = glm::vec3(viewDistance * cos(viewAzimuth), viewDistance * cos(viewPolar), viewDistance * sin(viewAzimuth));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(eye_center, lookat, up);
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix;

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);

        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));
        glm::mat4 mvp = projectionMatrix * viewNoTranslation;

        terrain.render();
        building.render(mvpMatrix);

        glDepthMask(GL_FALSE);  // Disable depth writing
        skybox.render(mvp);
        glDepthMask(GL_TRUE);   // Re-enable depth writing

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    skybox.cleanup();
    building.cleanup();
    glfwTerminate();

    return 0;
}
