/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Renderer.h
Description : Definitions for OpenGL rendering functions
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

#include "ModelLoader.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer(unsigned int Width, unsigned int Height, GLFWwindow* Window);
	void renderScene(GLuint ShaderProgram, const Model& Model, GLuint InstanceBuffer, GLuint InstanceCount,
	                 const std::vector<glm::mat4>& ModelMatrices);
	void renderMovingObject(GLuint ShaderProgram, const Model& MovingObjectModel);
	void renderUiElement(GLuint ShaderProgram) const;
	void processInput();
	Camera& getCamera();
	void updateWindowSize(int Width, int Height);

private:
	unsigned int MWidth;
	unsigned int MHeight;
	GLFWwindow* MWindow;
	glm::vec3 MObjectPosition;
	Camera MCamera;

	static void checkOpenGlError(const std::string& Stmt);
	static bool isMouseOverQuad(double MouseX, double MouseY, float QuadX, float QuadY, float QuadWidth,
	                            float QuadHeight);
	void processObjectMovement(float DeltaTime);
};
