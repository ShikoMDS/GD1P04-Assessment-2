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
#include "Renderer.h"
#include "UI.h"

// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
GLFWwindow* window;
Renderer* renderer; // Declare renderer as a global variable

// Function to initialize OpenGL and GLFW
bool initOpenGL(GLFWwindow*& window);

// Function to handle key events
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Function to check for OpenGL errors
void checkOpenGLError(const std::string& stmt);

int main() {
    if (!initOpenGL(window)) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    renderer = new Renderer(WIDTH, HEIGHT, window); // Initialize the renderer

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
        renderer->processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        renderer->renderScene(shaderProgram, model, instanceBuffer, instanceCount, modelMatrices);
        renderer->renderMovingObject(shaderProgram, movingObjectModel);
        renderer->renderUIElement(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer; // Clean up the renderer
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

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

// Key callback function
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Toggle camera mode with space key
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        renderer->getCamera().toggleMode();
    }
}

// Check OpenGL errors
void checkOpenGLError(const std::string& stmt) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << stmt << "): " << err << std::endl;
    }
}
