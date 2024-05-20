#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp> // For matrix printing
#include <glew.h>
#include <glfw3.h>
#include "ShaderLoader.h"
#include "ModelLoader.h"
#include "Camera.h"

// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
GLFWwindow* window;
glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Initialize the object's position

// UI Quad position and dimensions
const float quadX = 100.0f;
const float quadY = 100.0f;
const float quadWidth = 200.0f;
const float quadHeight = 200.0f;

// Function to initialize OpenGL and GLFW
bool initOpenGL(GLFWwindow*& window);

// Function to handle key events
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Function to check for OpenGL errors
void checkOpenGLError(const std::string& stmt);

void processInput(GLFWwindow* window);

Camera camera(20.0f, 1.0f);

// Function to process object movement
void processObjectMovement(GLFWwindow* window, float deltaTime) {
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

// Function to render the scene
void renderScene(GLuint shaderProgram, const Model& model, GLuint instanceBuffer, GLuint instanceCount, const std::vector<glm::mat4>& modelMatrices) {
    // Update camera
    float deltaTime = 0.016f; // assuming a fixed timestep for simplicity
    camera.update(deltaTime);

    // Handle camera input
    camera.processInput(window, deltaTime);

    // Use the camera matrices for rendering
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(WIDTH) / HEIGHT);
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

// Function to render the moving object
void renderMovingObject(GLuint shaderProgram, const Model& movingObjectModel) {
    // Update camera
    float deltaTime = 0.016f; // assuming a fixed timestep for simplicity
    camera.update(deltaTime);

    // Handle object movement
    processObjectMovement(window, deltaTime);

    // Use the camera matrices for rendering
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(WIDTH) / HEIGHT);
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

// Process input function
void processInput(GLFWwindow* window) {
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
        cameraModeToggled = false; //Needs to be false then true below for some reason or it won't work
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        cameraModeToggled = true;
    }
}

// Check if mouse is over the UI quad
bool isMouseOverQuad(double mouseX, double mouseY, float quadX, float quadY, float quadWidth, float quadHeight) {
    return mouseX >= quadX && mouseX <= quadX + quadWidth && mouseY >= quadY && mouseY <= quadY + quadHeight;
}

// Render UI element
void renderUIElement(GLuint shaderProgram) {
    // UI rendering setup
    glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f);
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

int main() {
    if (!initOpenGL(window)) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    GLuint shaderProgram = ShaderLoader::createProgram("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag");
    if (!shaderProgram) {
        std::cerr << "Failed to create shader program" << std::endl;
        return -1;
    }

    ModelLoader modelLoader;
    Model model = modelLoader.loadModel("resources/models/SciFiSpace/SM_Prop_Mine_01.obj");
    if (model.VAO == 0) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    Model movingObjectModel = modelLoader.loadModel("resources/models/SciFiSpace/SM_Ship_Fighter_02.obj");
    if (movingObjectModel.VAO == 0) {
        std::cerr << "Failed to load moving object model" << std::endl;
        return -1;
    }

    const unsigned int instanceCount = 1000;
    std::vector<glm::mat4> modelMatrices(instanceCount);
    srand(static_cast<unsigned int>(time(0)));

    for (unsigned int i = 0; i < instanceCount; i++) {
        float angle = static_cast<float>(rand() % 360);

        // Ensure displacement provides a wide spread
        float displacementX = static_cast<float>((rand() % 200) - 100) / 10.0f;
        float displacementY = static_cast<float>((rand() % 200) - 100) / 10.0f;
        float displacementZ = static_cast<float>((rand() % 200) - 100) / 10.0f;

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(displacementX, displacementY, displacementZ));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        float scale = static_cast<float>((rand() % 20) + 5) / 1000.0f;
        scale *= 0.5f; // Scale down the objects
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        modelMatrices[i] = modelMatrix;
    }

    unsigned int instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    glBindVertexArray(model.VAO); // Ensure VAO is bound before setting vertex attributes
    for (unsigned int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(2 + i, 1);
    }
    glBindVertexArray(0); // Unbind VAO

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        renderScene(shaderProgram, model, instanceBuffer, instanceCount, modelMatrices);
        renderMovingObject(shaderProgram, movingObjectModel);
        renderUIElement(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Function to initialize OpenGL
bool initOpenGL(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Demo 2", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glEnable(GL_CULL_FACE);  // Enable face culling
    glCullFace(GL_BACK);     // Cull back faces
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set the clear color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    return true;
}

// Key callback function
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Toggle camera mode with space key
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera.toggleMode();
    }
}

// Check OpenGL errors
void checkOpenGLError(const std::string& stmt) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << stmt << "): " << err << std::endl;
    }
}
