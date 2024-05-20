#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class Camera {
public:
    Camera(float radius, float speed);
    void toggleMode();
    void update(float deltaTime);
    void processInput(GLFWwindow* window, float deltaTime);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

private:
    void updateViewMatrix();
    void updatePosition();

    float radius;
    float speed;
    float angle;
    float pitch;
    bool automatic;
    float shiftMultiplier;
    glm::mat4 viewMatrix;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
};
