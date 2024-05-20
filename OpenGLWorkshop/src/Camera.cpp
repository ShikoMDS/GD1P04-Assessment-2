/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Camera.cpp
Description : Implementations for OpenGL camera functionality
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "Camera.h"

Camera::Camera(const float Radius, const float Speed)
	: MRadius(Radius), MSpeed(Speed), MAngle(0.0f), MAutomatic(false), MShiftMultiplier(2.0f),
	  MTarget(0.0f, 0.0f, 0.0f), MUp(0.0f, 1.0f, 0.0f)
{
	updatePosition();
	updateViewMatrix();
}

void Camera::toggleMode()
{
	MAutomatic = !MAutomatic;
}

void Camera::update(const float DeltaTime)
{
	if (MAutomatic)
	{
		const float ReducedSpeed = MSpeed * 0.1f; // Camera speed adjustment
		MAngle += ReducedSpeed * DeltaTime * MShiftMultiplier;
		updatePosition();
		updateViewMatrix();
	}
}

void Camera::processInput(GLFWwindow* Window, const float DeltaTime)
{
	MShiftMultiplier = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 2.0f : 1.0f;

	// Rotation clockwise around origin with Y-axis on X and Z axis plane
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		MAngle += MSpeed * DeltaTime * MShiftMultiplier;
	}
	// Rotation anti-clockwise around origin with Y-axis on X and Z axis plane
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		MAngle -= MSpeed * DeltaTime * MShiftMultiplier;
	}
	// Shorten radius (closer distance to world origin)
	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		MRadius -= MSpeed * DeltaTime * MShiftMultiplier * 5.0f;
		if (MRadius < 1.0f) MRadius = 1.0f; // Prevent the radius from going negative or too close
	}
	// Lengthen radius (longer distance to world origin)
	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		MRadius += MSpeed * DeltaTime * MShiftMultiplier * 5.0f;
	}

	updatePosition();
	updateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() const
{
	return MViewMatrix;
}

glm::mat4 Camera::getProjectionMatrix(const float AspectRatio) const
{
	return glm::perspective(glm::radians(45.0f), AspectRatio, 0.1f, 100.0f); // FOV is 45 degrees
}

void Camera::updateViewMatrix()
{
	MViewMatrix = lookAt(MPosition, MTarget, MUp);
}

void Camera::updatePosition()
{
	MPosition = glm::vec3(MRadius * cos(MAngle), 0.0f, MRadius * sin(MAngle));
}
