/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : main.cpp
Description : Main entry point for OpenGL Demo 2 GD1P04 Assessment 2
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>
#include <glew.h>
#include <glfw3.h>
#include "ShaderLoader.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "Renderer.h"
// TODO: Input A, Input A+

#include "UI.h"

// Window dimensions
constexpr unsigned int Width = 800;
constexpr unsigned int Height = 600;
GLFWwindow* GWindow;
Renderer* GRenderer;

// Function to initialise OpenGL and GLFW
bool initOpenGl(GLFWwindow*& Window);

// Function to handle key events
void keyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods);

// Function to check for OpenGL errors
void checkOpenGlError(const std::string& Stmt);

// Frame buffer size callback function
void frameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);

int main()
{
    if (!initOpenGl(GWindow))
    {
        std::cerr << "Failed to initialise OpenGL" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(GWindow, keyCallback);
    glfwSetFramebufferSizeCallback(GWindow, frameBufferSizeCallback); // Set the framebuffer size callback

    GRenderer = new Renderer(Width, Height, GWindow); // Initialise the renderer

    const GLuint ShaderProgram = ShaderLoader::createProgram("resources/shaders/VertexShader.vert",
        "resources/shaders/FragmentShader.frag");
    if (!ShaderProgram)
    {
        std::cerr << "Failed to create shader program" << std::endl;
        return -1;
    }

    constexpr ModelLoader LModelLoader;
    const Model LModel = LModelLoader.loadModel("resources/models/SciFiSpace/SM_Prop_Mine_01.obj",
        "resources/textures/PolygonSciFiSpace_Texture_01_A.png");
    if (LModel.Vao == 0)
    {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    const Model MovingObjectModel = LModelLoader.loadModel("resources/models/SciFiSpace/SM_Ship_Fighter_02.obj",
        "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    if (MovingObjectModel.Vao == 0)
    {
        std::cerr << "Failed to load moving object model" << std::endl;
        return -1;
    }

    constexpr unsigned int InstanceCount = 1000;
    std::vector<glm::mat4> ModelMatrices(InstanceCount);

    std::random_device Rd; // Random device for seed
    std::mt19937 Gen(Rd()); // Mersenne Twister random number generator
    std::uniform_real_distribution<float> AngleDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> DisplacementDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> ScaleDist(0.005f, 0.01f);

    for (unsigned int I = 0; I < InstanceCount; I++)
    {
        const float Angle = AngleDist(Gen);

        const float DisplacementX = DisplacementDist(Gen);
        const float DisplacementY = DisplacementDist(Gen);
        const float DisplacementZ = DisplacementDist(Gen);

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(DisplacementX, DisplacementY, DisplacementZ));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
        const float Scale = ScaleDist(Gen);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(Scale));

        ModelMatrices[I] = ModelMatrix;
    }

    unsigned int InstanceBuffer;
    glGenBuffers(1, &InstanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, InstanceCount * sizeof(glm::mat4), ModelMatrices.data(), GL_STATIC_DRAW);

    // Ensure VAO is bound before setting vertex attributes
    glBindVertexArray(LModel.Vao);
    for (unsigned int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
            reinterpret_cast<void*>(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(2 + i, 1);
    }
    glBindVertexArray(0); // Unbind VAO

    while (!glfwWindowShouldClose(GWindow))
    {
        GRenderer->processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(ShaderProgram);

        // Bind the texture for the main model
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, LModel.Texture);
        glUniform1i(glGetUniformLocation(ShaderProgram, "textureSampler"), 0);
        GRenderer->renderScene(ShaderProgram, LModel, InstanceBuffer, InstanceCount, ModelMatrices);

        // Bind the texture for the moving object
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MovingObjectModel.Texture);
        glUniform1i(glGetUniformLocation(ShaderProgram, "textureSampler"), 0);
        GRenderer->renderMovingObject(ShaderProgram, MovingObjectModel);

        GRenderer->renderUiElement(ShaderProgram);

        glfwSwapBuffers(GWindow);
        glfwPollEvents();
    }

    delete GRenderer; // Clean up renderer
    glfwTerminate();
    return 0;
}

// Function to initialise OpenGL
bool initOpenGl(GLFWwindow*& Window)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialise GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window = glfwCreateWindow(Width, Height, "OpenGL Demo 2", nullptr, nullptr);
	if (!Window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(Window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialise GLEW" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_CULL_FACE); // Enable culling
	glCullFace(GL_BACK); // Cull back faces
	glViewport(0, 0, Width, Height);

	// Set window color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

// Key callback function
void keyCallback(GLFWwindow* Window, const int Key, int ScanCode, const int Action, int Mods)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, GLFW_TRUE);

	// Toggle camera mode with space key
	if (Key == GLFW_KEY_SPACE && Action == GLFW_PRESS)
	{
		GRenderer->getCamera().toggleMode();
	}
}

// Frame buffer size callback function
void frameBufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
	glViewport(0, 0, Width, Height);
	GRenderer->updateWindowSize(Width, Height);
	// Add this line for renderer since it needs to be aware of the new window size
}

// Check OpenGL errors
void checkOpenGlError(const std::string& Stmt)
{
	const GLenum Err = glGetError();
	if (Err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error (" << Stmt << "): " << Err << std::endl;
	}
}
