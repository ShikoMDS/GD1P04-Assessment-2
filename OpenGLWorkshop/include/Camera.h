/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Camera.h
Description : Definitions for OpenGL camera functionality
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class Camera
{
public:
	Camera(float Radius, float Speed);
	void toggleMode();
	void update(float DeltaTime);
	void processInput(GLFWwindow* Window, float DeltaTime);
	[[nodiscard]] glm::mat4 getViewMatrix() const;
	[[nodiscard]] glm::mat4 getProjectionMatrix(float AspectRatio) const;

private:
	void updateViewMatrix();
	void updatePosition();

	float MRadius;
	float MSpeed;
	float MAngle;
	bool MAutomatic;
	float MShiftMultiplier;
	glm::mat4 MViewMatrix{};
	glm::vec3 MPosition{};
	glm::vec3 MTarget;
	glm::vec3 MUp;
};
