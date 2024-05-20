#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

class UI {
public:
    UI();
    void renderUIElement(GLuint shaderProgram, float width, float height, GLFWwindow* window);

private:
    bool isMouseOverQuad(double mouseX, double mouseY, float quadX, float quadY, float quadWidth, float quadHeight);
};
