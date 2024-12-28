#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <tiny_gltf.h>
#include <render/shader.h>
#include <vector>
#include <iostream>
#include <map>
#include <cassert>
#include <cstring>
#include <sstream>
#include <iomanip>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct MyBot {
    // Shader uniform IDs
    GLuint mvpMatrixID;
    GLuint jointMatricesID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint programID;

    // Light properties
    glm::vec3 lightIntensity;
    glm::vec3 lightPosition;

    // Constructor initializes light properties
    MyBot() : lightIntensity(5e6f, 5e6f, 5e6f),
             lightPosition(-275.0f, 500.0f, 800.0f) {}

    // GLTF model and animation data
    tinygltf::Model model;

    // Animation control properties
    float loopStartTime = 0.5f;
    float loopEndTime = 10.0f;
    bool useLooping = true;

    // Structures for GLTF rendering and animation
    struct PrimitiveObject {
        GLuint vao;
        std::map<int, GLuint> vbos;
    };
    std::vector<PrimitiveObject> primitiveObjects;

    struct SkinObject {
        std::vector<glm::mat4> inverseBindMatrices;
        std::vector<glm::mat4> globalJointTransforms;
        std::vector<glm::mat4> jointMatrices;
    };
    std::vector<SkinObject> skinObjects;

    struct SamplerObject {
        std::vector<float> input;
        std::vector<glm::vec4> output;
        int interpolation;
    };

    struct TextureObject {
        GLuint id;
        int width;
        int height;
        int channels;
        std::string type;
    };

    struct AnimationObject {
        std::vector<SamplerObject> samplers;
    };
    std::vector<AnimationObject> animationObjects;

    // Inverse root transformation for skinning
    glm::mat4 inverseRootTransform;

    // Texture IDs
    GLuint diffuseMapID;
    GLuint normalMapID;
    GLuint aoMapID;
    std::vector<TextureObject> textureObjects;

    // Methods for loading and managing textures
    GLuint loadTexture(const tinygltf::Image& image);
    void loadMaterialTextures(const tinygltf::Model& model, const tinygltf::Material& material);

    // Methods for node transformations
    glm::mat4 getNodeTransform(const tinygltf::Node& node);
    void computeLocalNodeTransform(const tinygltf::Model& model, int nodeIndex, std::vector<glm::mat4>& localTransforms);
    void computeGlobalNodeTransform(const tinygltf::Model& model, const std::vector<glm::mat4>& localTransforms, int nodeIndex, const glm::mat4& parentTransform, std::vector<glm::mat4>& globalTransforms);

    // Methods for skinning and animation
    std::vector<SkinObject> prepareSkinning(const tinygltf::Model& model);
    int findKeyframeIndex(const std::vector<float>& times, float animationTime);
    std::vector<AnimationObject> prepareAnimation(const tinygltf::Model& model);

    void updateAnimation(const tinygltf::Model& model, const tinygltf::Animation& anim, const AnimationObject& animationObject, float time, std::vector<glm::mat4>& nodeTransforms);
    void updateSkinning(const tinygltf::Skin& skin, const std::vector<glm::mat4>& nodeTransforms);

    void update(float time);
    bool loadModel(tinygltf::Model& model, const char* filename);
    void initialize();

    // Methods for binding and drawing GLTF data
    void bindMesh(std::vector<PrimitiveObject>& primitiveObjects, tinygltf::Model& model, tinygltf::Mesh& mesh);
    void bindModelNodes(std::vector<PrimitiveObject>& primitiveObjects, tinygltf::Model& model, tinygltf::Node& node);
    std::vector<PrimitiveObject> bindModel(tinygltf::Model& model);

    void drawMesh(const std::vector<PrimitiveObject>& primitiveObjects, tinygltf::Model& model, tinygltf::Mesh& mesh);
    void drawModelNodes(const std::vector<PrimitiveObject>& primitiveObjects, tinygltf::Model& model, tinygltf::Node& node);
    void drawModel(const std::vector<PrimitiveObject>& primitiveObjects, tinygltf::Model& model);

    // Rendering and cleanup
    void render(glm::mat4 cameraMatrix);
    void cleanup();
};
