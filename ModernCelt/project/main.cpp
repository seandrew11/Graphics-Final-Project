#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Building.h"
#include "Skybox.h"
#include "Terrain.h"
#include "render/shader.h"
#include "Character.h"
#include "stb_image.h"

// Global Variables
GLFWwindow* window;
static bool playAnimation = true;
static float playbackSpeed = 1.0f;
static float characterTime = 0.0f;
static double lastTime = glfwGetTime();

// Camera variables
static glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 75.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static float yaw = -90.0f;
static float pitch = 0.0f;
static glm::vec3 up(0, 1, 0);

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    float cameraSpeed = 1.0f;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        pitch += 2.0f;
        if(pitch > 89.0f) pitch = 89.0f;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        pitch -= 2.0f;
        if(pitch < -89.0f) pitch = -89.0f;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        yaw -= 2.0f;
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        yaw += 2.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

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

    GLuint buildingTexture1 = LoadTextureTileBox("../project/textures/alien2.jpg");
    GLuint buildingTexture2 = LoadTextureTileBox("../project/textures/facade3.jpg");

    MyBot character1, character2;
    character1.initialize();
    character2.initialize();

    Building building, pub;
    building.initialize(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(5.0f, 40.0f, 5.0f), buildingTexture1);
    pub.initialize(glm::vec3(-10.0f, 3.0f, -35.0f), glm::vec3(12.0f, 16.0f, 5.0f), buildingTexture2);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix;

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);

        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));
        glm::mat4 mvp = projectionMatrix * viewNoTranslation;

        terrain.render();
        building.render(mvpMatrix);
        pub.render(mvpMatrix);

        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if (playAnimation) {
            characterTime += deltaTime * playbackSpeed;
            character1.update(characterTime);
            character2.update(characterTime);
        }

        glm::mat4 characterModelMatrix = glm::translate(glm::mat4(1.0f),
            glm::vec3(-15.0f, terrain.getHeight(-47 + 250, -47 + 250), -15.0f));
        characterModelMatrix = glm::rotate(characterModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        characterModelMatrix = glm::scale(characterModelMatrix, glm::vec3(0.05f));
        glm::mat4 characterMVP = mvpMatrix * characterModelMatrix;
        character1.render(characterMVP);

        glm::mat4 characterModelMatrix2 = glm::translate(glm::mat4(1.0f),
            glm::vec3(-5.0f, terrain.getHeight(-47 + 250, -47 + 250), -20.0f));
        characterModelMatrix2 = glm::rotate(characterModelMatrix2, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        characterModelMatrix2 = glm::scale(characterModelMatrix2, glm::vec3(0.05f));
        glm::mat4 characterMVP2 = mvpMatrix * characterModelMatrix2;
        character2.render(characterMVP2);

        glDepthMask(GL_FALSE);
        skybox.render(mvp);
        glDepthMask(GL_TRUE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    skybox.cleanup();
    building.cleanup();
    pub.cleanup();
    character1.cleanup();
    character2.cleanup();
    glfwTerminate();
    return 0;
}
