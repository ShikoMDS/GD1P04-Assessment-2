/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Renderer.cpp
Description : Implementations for OpenGL rendering functions
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "Renderer.h"

// UI Quad position and dimensions
constexpr float QuadX = 100.0f;
constexpr float QuadY = 100.0f;
constexpr float QuadWidth = 200.0f;
constexpr float QuadHeight = 200.0f;

Renderer::Renderer(const unsigned int Width, const unsigned int Height, GLFWwindow* Window)
	: MWidth(Width), MHeight(Height), MWindow(Window), MObjectPosition(0.0f, 0.0f, 0.0f), MCamera(20.0f, 1.0f)
{
}

void Renderer::renderScene(const GLuint ShaderProgram, const Model& Model, GLuint InstanceBuffer,
                           const GLuint InstanceCount, const std::vector<glm::mat4>& ModelMatrices)
{
	// Update camera
	constexpr float DeltaTime = 0.005f; // Fix time for speed
	MCamera.update(DeltaTime);

	// Handle camera input
	MCamera.processInput(MWindow, DeltaTime);

	// Use the camera matrices for rendering
	const glm::mat4 Projection = MCamera.getProjectionMatrix(static_cast<float>(MWidth) / static_cast<float>(MHeight));
	const glm::mat4 View = MCamera.getViewMatrix();

	// Render instanced objects
	glBindVertexArray(Model.Vao);
	for (unsigned int I = 0; I < InstanceCount; I++)
	{
		glm::mat4 Mvp = Projection * View * ModelMatrices[I];
		const GLint MvpLocation = glGetUniformLocation(ShaderProgram, "mvp");
		if (MvpLocation == -1)
		{
			std::cerr << "Could not find uniform location for 'mvp'" << std::endl;
		}
		else
		{
			glUniformMatrix4fv(MvpLocation, 1, GL_FALSE, value_ptr(Mvp));
		}
		glDrawElements(GL_TRIANGLES, Model.IndexCount, GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);

	checkOpenGlError("renderScene");
}

void Renderer::renderMovingObject(const GLuint ShaderProgram, const Model& MovingObjectModel)
{
	// Update camera
	constexpr float DeltaTime = 0.01f; // Fix time for speed
	MCamera.update(DeltaTime);

	// Handle object movement
	processObjectMovement(DeltaTime);

	// Use the camera matrices for rendering
	const glm::mat4 Projection = MCamera.getProjectionMatrix(static_cast<float>(MWidth) / static_cast<float>(MHeight));
	const glm::mat4 View = MCamera.getViewMatrix();
	glm::mat4 ModelMatrix = translate(glm::mat4(1.0f), MObjectPosition);

	// Scale down the moving object
	ModelMatrix = scale(ModelMatrix, glm::vec3(0.001f));

	// Rotate the object to face the screen (+Z)
	ModelMatrix = rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 Mvp = Projection * View * ModelMatrix;
	const GLint MvpLocation = glGetUniformLocation(ShaderProgram, "mvp");
	if (MvpLocation == -1)
	{
		std::cerr << "Could not find uniform location for 'mvp'" << std::endl;
	}
	else
	{
		glUniformMatrix4fv(MvpLocation, 1, GL_FALSE, value_ptr(Mvp));
	}

	// Bind the model's VAO and draw
	glBindVertexArray(MovingObjectModel.Vao);
	glDrawElements(GL_TRIANGLES, MovingObjectModel.IndexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	checkOpenGlError("renderMovingObject");
}

void Renderer::renderUiElement(const GLuint ShaderProgram) const
{
	// UI rendering setup
	const glm::mat4 OrthoProjection = glm::ortho(0.0f, static_cast<float>(MWidth), static_cast<float>(MHeight), 0.0f);
	constexpr auto Model = glm::mat4(1.0f);
	glm::mat4 Mvp = OrthoProjection * Model;

	// Pass mvp to shader and render quad
	const GLint MvpLocation = glGetUniformLocation(ShaderProgram, "mvp");
	glUniformMatrix4fv(MvpLocation, 1, GL_FALSE, value_ptr(Mvp));

	double Xpos, Ypos;
	glfwGetCursorPos(MWindow, &Xpos, &Ypos);
	if (isMouseOverQuad(Xpos, Ypos, QuadX, QuadY, QuadWidth, QuadHeight))
	{
		// TODO: Input A
		// TODO: Input A+
		// Change texture on hover
	}
}

void Renderer::processInput()
{
	// Debouncing
	static bool CursorToggled = false;
	static bool WireframeToggled = false;
	static bool MousePositionLogged = false;
	static bool CameraModeToggled = false;

	// Cursor visibility toggle (1)
	if (glfwGetKey(MWindow, GLFW_KEY_1) == GLFW_PRESS && !CursorToggled)
	{
		static bool CursorVisible = true;
		CursorVisible = !CursorVisible;
		glfwSetInputMode(MWindow, GLFW_CURSOR, CursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		CursorToggled = true;
	}
	if (glfwGetKey(MWindow, GLFW_KEY_1) == GLFW_RELEASE)
	{
		CursorToggled = false;
	}

	// Wireframe mode toggle (2) 
	if (glfwGetKey(MWindow, GLFW_KEY_2) == GLFW_PRESS && !WireframeToggled)
	{
		static bool WireframeMode = false;
		WireframeMode = !WireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, WireframeMode ? GL_LINE : GL_FILL);
		WireframeToggled = true;
	}
	if (glfwGetKey(MWindow, GLFW_KEY_2) == GLFW_RELEASE)
	{
		WireframeToggled = false;
	}

	// Cursor position log (3)
	if (glfwGetKey(MWindow, GLFW_KEY_3) == GLFW_PRESS && !MousePositionLogged)
	{
		double Xpos, Ypos;
		glfwGetCursorPos(MWindow, &Xpos, &Ypos);
		std::cout << "Cursor Position: (" << Xpos << ", " << Ypos << ")\n";
		MousePositionLogged = true;
	}
	if (glfwGetKey(MWindow, GLFW_KEY_3) == GLFW_RELEASE)
	{
		MousePositionLogged = false;
	}

	// Toggle automatic camera (space)
	if (glfwGetKey(MWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !CameraModeToggled)
	{
		MCamera.toggleMode();
		CameraModeToggled = false;
		// Needs to be false then true below for some reason or it won't work. Maybe the way I coded it?
	}
	if (glfwGetKey(MWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		CameraModeToggled = true;
	}
}

void Renderer::processObjectMovement(const float DeltaTime)
{
	const float MovementSpeed = 5.0f * DeltaTime;

	// *ISSUES* with implementing movement based off the camera's view. Opted for fixed.
	// Fix movement regardless of camera rotation
	constexpr auto WorldX = glm::vec3(0.0f, 0.0f, -1.0f); // Fixed forward (X) direction
	constexpr auto WorldZ = glm::vec3(1.0f, 0.0f, 0.0f); // Fixed right (Z) direction
	constexpr auto WorldY = glm::vec3(0.0f, 1.0f, 0.0f); // Fixed up (Y) direction

	// +X movement
	if (glfwGetKey(MWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		MObjectPosition += WorldX * MovementSpeed;
	}
	// -X movement
	if (glfwGetKey(MWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		MObjectPosition -= WorldX * MovementSpeed;
	}
	// -Z movement
	if (glfwGetKey(MWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		MObjectPosition -= WorldZ * MovementSpeed;
	}
	// +Z movement
	if (glfwGetKey(MWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		MObjectPosition += WorldZ * MovementSpeed;
	}
	// +Y movement
	if (glfwGetKey(MWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		MObjectPosition += WorldY * MovementSpeed;
	}
	// -Y movement
	if (glfwGetKey(MWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		MObjectPosition -= WorldY * MovementSpeed;
	}
}

bool Renderer::isMouseOverQuad(const double MouseX, const double MouseY, const float QuadX, const float QuadY,
                               const float QuadWidth, const float QuadHeight)
{
	return MouseX >= QuadX && MouseX <= QuadX + QuadWidth && MouseY >= QuadY && MouseY <= QuadY + QuadHeight;
}

void Renderer::checkOpenGlError(const std::string& Stmt)
{
	const GLenum Err = glGetError();
	if (Err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error (" << Stmt << "): " << Err << std::endl;
	}
}

Camera& Renderer::getCamera()
{
	return MCamera;
}

void Renderer::updateWindowSize(const int Width, const int Height)
{
	this->MWidth = Width;
	this->MHeight = Height;
}
