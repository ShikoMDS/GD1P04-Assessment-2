#include "UI.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const float quadX = 100.0f;
const float quadY = 100.0f;
const float quadWidth = 200.0f;
const float quadHeight = 200.0f;

UI::UI() {}

void UI::renderUIElement(GLuint shaderProgram, float width, float height, GLFWwindow* window) {
    // UI rendering setup
    glm::mat4 orthoProjection = glm::ortho(0.0f, width, height, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = orthoProjection * model;

    // Pass mvp to shader and render quad
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (isMouseOverQuad(xpos, ypos, quadX, quadY, quadWidth, quadHeight)) {
        // Change texture on hover
    }
}

bool UI::isMouseOverQuad(double mouseX, double mouseY, float quadX, float quadY, float quadWidth, float quadHeight) {
    return mouseX >= quadX && mouseX <= quadX + quadWidth && mouseY >= quadY && mouseY <= quadY + quadHeight;
}
