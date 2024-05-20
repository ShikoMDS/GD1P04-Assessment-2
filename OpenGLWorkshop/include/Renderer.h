#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "ModelLoader.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height, GLFWwindow* window);
    void renderScene(GLuint shaderProgram, const Model& model, GLuint instanceBuffer, GLuint instanceCount, const std::vector<glm::mat4>& modelMatrices);
    void renderMovingObject(GLuint shaderProgram, const Model& movingObjectModel);
    void renderUIElement(GLuint shaderProgram);
    void processInput();
    Camera& getCamera();

private:
    unsigned int width;
    unsigned int height;
    GLFWwindow* window;
    glm::vec3 objectPosition;
    Camera camera;

    void checkOpenGLError(const std::string& stmt);
    bool isMouseOverQuad(double mouseX, double mouseY, float quadX, float quadY, float quadWidth, float quadHeight);
    void processObjectMovement(float deltaTime);
};
