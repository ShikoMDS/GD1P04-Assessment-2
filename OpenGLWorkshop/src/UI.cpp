/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Ui.cpp
Description : Implementations for UI functionality in OpenGL
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "UI.h"

constexpr float QuadX = 100.0f;
constexpr float QuadY = 100.0f;
constexpr float QuadWidth = 200.0f;
constexpr float QuadHeight = 200.0f;

Ui::Ui() = default;

void Ui::renderUiElement(const GLuint ShaderProgram, const float Width, const float Height, GLFWwindow* Window) const
{
	// UI rendering setup
	const glm::mat4 OrthoProjection = glm::ortho(0.0f, static_cast<float>(Width), 0.0f, static_cast<float>(Height));
	constexpr auto Model = glm::mat4(1.0f);
	glm::mat4 Mvp = OrthoProjection * Model;

	// Pass MVP to shader and render quad
	const GLuint MvpLocation = glGetUniformLocation(ShaderProgram, "mvp");
	glUniformMatrix4fv(MvpLocation, 1, GL_FALSE, value_ptr(Mvp));

	double Xpos, Ypos;
	glfwGetCursorPos(Window, &Xpos, &Ypos);
	if (isMouseOverQuad(Xpos, Ypos, QuadX, QuadY, QuadWidth, QuadHeight))
	{
		// Change texture on hover
	}
}

bool Ui::isMouseOverQuad(const double MouseX, const double MouseY, const float QuadX, const float QuadY,
                         const float QuadWidth, const float QuadHeight)
{
	return MouseX >= QuadX && MouseX <= QuadX + QuadWidth && MouseY >= QuadY && MouseY <= QuadY + QuadHeight;
}

/*
TODO: Input A
A textured quad, rendered as a UI element using an orthographic projection matrix is
present. The texture of the quad changes when the user hovers the mouse over the quad
and reverts to the original texture upon the mouse leaving the quad's screen space.

TODO: Input A+
Clicking the UI element results in the instanced objects changing the texture they are
rendered with.
*/
