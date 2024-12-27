#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image_write.h>

#include "Building.h"
#include "Skybox.h"
#include "Terrain.h"
#include "render/shader.h"
#include "Character.h"
#include "IrishPub.h"
#include "stb_image.h"

// Global Variables
GLFWwindow* window;
static bool playAnimation = true;
static float playbackSpeed = 1.0f;
static float characterTime = 0.0f;
static double lastTime = glfwGetTime();
static bool saveDepth = false;

// Camera variables
static glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 75.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static float yaw = -90.0f;
static float pitch = 0.0f;
static glm::vec3 up(0, 1, 0);
const glm::vec3 wave500(0.0f, 255.0f, 146.0f);
const glm::vec3 wave600(255.0f, 190.0f, 0.0f);
const glm::vec3 wave700(205.0f, 0.0f, 0.0f);
static glm::vec3 lightPosition(-200.0f, 100.0f, 200.0f);
static glm::vec3 lightIntensity = 3.0f * (wave500 + wave600 + wave700);
static GLuint depthMapFBO;
static GLuint depthMap;
const GLuint SHADOW_WIDTH = 4096;
const GLuint SHADOW_HEIGHT = 4096;
static GLuint depthShaderProg;

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        saveDepth = true;
    }
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

void saveDepthTexture(GLuint fbo, std::string filename) {
    int width = SHADOW_WIDTH;
    int height = SHADOW_HEIGHT;
    int channels = 1;

    std::vector<float> depth(width * height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert depth values to grayscale image
    std::vector<unsigned char> img(width * height);
    for (int i = 0; i < width * height; ++i) {
        img[i] = static_cast<unsigned char>(depth[i] * 255);
    }

    stbi_write_png(filename.c_str(), width, height, channels, img.data(), width * channels);
}

void initializeShadowMap() {
    // Generate framebuffer
    glGenFramebuffers(1, &depthMapFBO);

    // Create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Shadow Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    glEnable(GL_CULL_FACE);

    // Setup shaders
    GLuint shaderProgram = LoadShadersFromFile("../project/terrain.vert", "../project/terrain.frag");
    if (shaderProgram == 0) {
        std::cerr << "Failed to load shaders." << std::endl;
        return -1;
    }

    // Initialize objects

    initializeShadowMap();
    depthShaderProg = LoadShadersFromFile("../project/depth.vert", "../project/depth.frag");

    Skybox skybox;
    skybox.initialize(glm::vec3(0.0f), glm::vec3(500.0f));


    Terrain terrain(500, 500, shaderProgram, glm::vec3(0.0f, 0.0f, 0.0f));

    GLuint terrainTexture = LoadTextureTileBox("../project/textures/Grass_01.png");
    GLuint terrainSampler = glGetUniformLocation(shaderProgram, "terrainTexture");
    terrain.setTexture(terrainTexture, terrainSampler);

    GLuint buildingTexture1 = LoadTextureTileBox("../project/textures/alien2.jpg");
    GLuint pubside = LoadTextureTileBox("../project/textures/facade3.jpg");
    GLuint pubfront = LoadTextureTileBox("../project/textures/pub1.jpg");

    MyBot character1, character2;
    character1.initialize();
    character2.initialize();

    Building building;
    IrishPub pub;
    building.initialize(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(5.0f, 40.0f, 5.0f), buildingTexture1);
    pub.initialize(glm::vec3(-10.0f, -5.0f, -35.0f), glm::vec3(12.0f, 16.0f, 5.0f), pubfront, pubside, lightPosition,    // Add these parameters
               lightIntensity);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // First pass: Render depth from light's perspective
        glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f,
                                          -200.0f, 200.0f,
                                          1.0f, 1000.0f);
    glm::mat4 lightView = glm::lookAt(lightPosition,
                                     glm::vec3(-10.0f, 0.0f, -20.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    // Render to depth map
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Use depth shader for shadow pass
    glUseProgram(depthShaderProg);
    glUniformMatrix4fv(glGetUniformLocation(depthShaderProg, "lightSpaceMatrix"),
                       1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Render objects for shadow mapping
    terrain.renderDepth(lightSpaceMatrix);
    building.renderDepth(lightSpaceMatrix);
    pub.renderDepth(lightSpaceMatrix);

        if (saveDepth) {
            std::string filename = "depth_map.png";
            saveDepthTexture(depthMapFBO, filename);
            std::cout << "Depth texture saved to " << filename << std::endl;
            saveDepth = false;
        }

    // Second pass: Normal rendering
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
    glm::mat4 mvpMatrix = projectionMatrix * viewMatrix;
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));
    glm::mat4 mvp = projectionMatrix * viewNoTranslation;

    // Bind shadow map for normal rendering
    glActiveTexture(GL_TEXTURE1); // Use different texture unit than regular textures
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // Your existing render calls, but passing lightSpaceMatrix
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(glm::mat4(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE,
                       glm::value_ptr(lightSpaceMatrix));
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 1); // Texture unit 1

        terrain.updateTerrain(cameraPos);
        terrain.render(mvpMatrix, lightPosition, lightIntensity, lightSpaceMatrix);
        building.render(mvpMatrix, lightPosition, lightIntensity, lightSpaceMatrix);
        pub.render(mvpMatrix, lightSpaceMatrix);

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
