/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : ShaderLoader.h
Description : Definitions for shader loading in OpenGL
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader();

	// Delete the copy constructor and copy assignment operator
	ShaderLoader(const ShaderLoader&) = delete;
	ShaderLoader& operator=(const ShaderLoader&) = delete;

	// Delete the move constructor and move assignment operator
	ShaderLoader(ShaderLoader&&) = delete;
	ShaderLoader& operator=(ShaderLoader&&) = delete;

	static GLuint createProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

private:
	static GLuint createShader(GLenum ShaderType, const char* ShaderName);
	static std::string readShaderFile(const char* Filename);
	static void printErrorDetails(bool isShader, GLuint Id, const char* Name);
};
