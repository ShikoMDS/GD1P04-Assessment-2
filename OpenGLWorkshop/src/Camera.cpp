#include "Camera.h"

Camera::Camera(float radius, float speed)
    : radius(radius), speed(speed), angle(0.0f), pitch(0.0f), automatic(false), shiftMultiplier(1.0f), target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) {
    updatePosition();
    updateViewMatrix();
}

void Camera::toggleMode() {
    automatic = !automatic;
}

void Camera::update(float deltaTime) {
    if (automatic) {
        angle += speed * deltaTime * shiftMultiplier;
        updatePosition();
        updateViewMatrix();
    }
}

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    shiftMultiplier = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 2.0f : 1.0f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angle -= speed * deltaTime * shiftMultiplier;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        angle += speed * deltaTime * shiftMultiplier;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        radius -= speed * deltaTime * shiftMultiplier;
        if (radius < 1.0f) radius = 1.0f; // Prevent the radius from going negative or too close
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        radius += speed * deltaTime * shiftMultiplier;
    }

    updatePosition();
    updateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); // Field of view is 45 degrees
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, target, up);
}

void Camera::updatePosition() {
    position = glm::vec3(radius * cos(angle), 0.0f, radius * sin(angle));
}
