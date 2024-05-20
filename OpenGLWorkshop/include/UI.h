/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Ui.h
Description : Definitions for UI functionality in OpenGL
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Ui
{
public:
	Ui();
	void renderUiElement(GLuint ShaderProgram, float Width, float Height, GLFWwindow* Window) const;

private:
	static bool isMouseOverQuad(double MouseX, double MouseY, float QuadX, float QuadY, float QuadWidth,
	                            float QuadHeight);
};
