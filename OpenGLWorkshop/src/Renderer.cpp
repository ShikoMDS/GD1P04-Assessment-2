#include "Renderer.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

// UI Quad position and dimensions
const float quadX = 100.0f;
const float quadY = 100.0f;
const float quadWidth = 200.0f;
const float quadHeight = 200.0f;

Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow* window)
    : width(width), height(height), window(window), objectPosition(0.0f, 0.0f, 0.0f), camera(20.0f, 1.0f) {}

void Renderer::renderScene(GLuint shaderProgram, const Model& model, GLuint instanceBuffer, GLuint instanceCount, const std::vector<glm::mat4>& modelMatrices) {
    // Update camera
    float deltaTime = 0.016f; // assuming a fixed timestep for simplicity
    camera.update(deltaTime);

    // Handle camera input
    camera.processInput(window, deltaTime);

    // Use the camera matrices for rendering
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(width) / height);
    glm::mat4 view = camera.getViewMatrix();

    // Render instanced objects
    glBindVertexArray(model.VAO);
    for (unsigned int i = 0; i < instanceCount; i++) {
        glm::mat4 mvp = projection * view * modelMatrices[i];
        GLuint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
        if (mvpLocation == -1) {
            std::cerr << "Could not find uniform location for 'mvp'" << std::endl;
        }
        else {
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        }
        glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    checkOpenGLError("renderScene");
}

void Renderer::renderMovingObject(GLuint shaderProgram, const Model& movingObjectModel) {
    // Update camera
    float deltaTime = 0.016f; // assuming a fixed timestep for simplicity
    camera.update(deltaTime);

    // Handle object movement
    processObjectMovement(deltaTime);

    // Use the camera matrices for rendering
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(width) / height);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), objectPosition);

    // Scale down the moving object
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001f)); // Adjust this value to scale the ship

    // Rotate the ship to face the screen
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Adjust the angle and axis as needed

    glm::mat4 mvp = projection * view * modelMatrix;
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    if (mvpLocation == -1) {
        std::cerr << "Could not find uniform location for 'mvp'" << std::endl;
    }
    else {
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    }

    // Bind the model's VAO and draw
    glBindVertexArray(movingObjectModel.VAO);
    glDrawElements(GL_TRIANGLES, movingObjectModel.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    checkOpenGLError("renderMovingObject");
}

void Renderer::renderUIElement(GLuint shaderProgram) {
    // UI rendering setup
    glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
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

void Renderer::processInput() {
    static bool cursorVisible = true;
    static bool wireframeMode = false;
    static bool cursorToggled = false;
    static bool wireframeToggled = false;
    static bool mousePositionLogged = false;
    static bool cameraModeToggled = false; // For camera mode toggle debounce

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !cursorToggled) {
        cursorVisible = !cursorVisible;
        glfwSetInputMode(window, GLFW_CURSOR, cursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        cursorToggled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
        cursorToggled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !wireframeToggled) {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        wireframeToggled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) {
        wireframeToggled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !mousePositionLogged) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Cursor Position: (" << xpos << ", " << ypos << ")\n";
        mousePositionLogged = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) {
        mousePositionLogged = false;
    }

    // Toggle camera mode with space key
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !cameraModeToggled) {
        camera.toggleMode();
        cameraModeToggled = false; // Needs to be false then true below for some reason or it won't work
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        cameraModeToggled = true;
    }
}

void Renderer::processObjectMovement(float deltaTime) {
    float movementSpeed = 5.0f * deltaTime;

    glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f); // Fixed forward direction
    glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);    // Fixed right direction
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);       // Fixed up direction

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        objectPosition += worldForward * movementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        objectPosition -= worldForward * movementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        objectPosition -= worldRight * movementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        objectPosition += worldRight * movementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        objectPosition += worldUp * movementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        objectPosition -= worldUp * movementSpeed;
    }
}

bool Renderer::isMouseOverQuad(double mouseX, double mouseY, float quadX, float quadY, float quadWidth, float quadHeight) {
    return mouseX >= quadX && mouseX <= quadX + quadWidth && mouseY >= quadY && mouseY <= quadY + quadHeight;
}

void Renderer::checkOpenGLError(const std::string& stmt) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << stmt << "): " << err << std::endl;
    }
}

Camera& Renderer::getCamera() {
    return camera;
}
