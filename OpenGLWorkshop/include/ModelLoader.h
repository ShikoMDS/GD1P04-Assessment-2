/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : ModelLoader.h
Description : Definitions for model loading in OpenGL through
			  tinyobjloader and STB
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glew.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

struct Model
{
	GLuint Vao;
	GLuint Vbo;
	GLuint Ebo;
	GLuint Texture;
	int IndexCount;
};

class ModelLoader
{
public:
	Model loadModel(const char* ModelPath, const char* TexturePath) const;

private:
	static void setupModel(Model& Model, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices);
	static GLuint loadTexture(const char* Path);
};
