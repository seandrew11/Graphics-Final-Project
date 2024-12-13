#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Terrain.h"
#include "render/shader.h"

GLFWwindow* window;

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

    // Load shaders
    GLuint shaderProgram = LoadShadersFromFile("../project/terrain.vert", "../project/terrain.frag");
    if (shaderProgram == 0) {
        std::cerr << "Failed to load shaders" << std::endl;
        return -1;
    }

    // Create terrain
    Terrain terrain(50, 50);

    // Set up projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.1f, 100.0f);

    // Set up view matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 50, 50),  // Move camera further back and higher up
        glm::vec3(0, 0, 0),    // 0,0,0 origin
        glm::vec3(0, 1, 0)     // Up vector
    );

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Background color
    glClearColor(0.2f, 0.2f, 0.25f, 0.0f);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader and set MVP matrix
        glUseProgram(shaderProgram);
        glm::mat4 MVP = projection * view * model;
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
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
