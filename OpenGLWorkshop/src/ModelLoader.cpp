/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : ModelLoader.cpp
Description : Implementations for model loading through tinyobjloader and
			  textures with STB following OpenGL rendering pipeline
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "ModelLoader.h"
#include "tiny_obj_loader.h"
#include "stb_image.h"

Model ModelLoader::loadModel(const char* ModelPath, const char* TexturePath) const
{
	Model Model = {};
	tinyobj::attrib_t Attrib;
	std::vector<tinyobj::shape_t> Shapes;
	std::vector<tinyobj::material_t> Materials;
	std::string Warn, Err;

	bool Ret = LoadObj(&Attrib, &Shapes, &Materials, &Warn, &Err, ModelPath);
	if (!Ret)
	{
		std::cerr << "Failed to load model: " << Err << std::endl;
		return Model;
	}

	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

	// Structured binding
	for (const auto& [name, mesh, lines, points] : Shapes)
	{
		for (const auto& [vertex_index, normal_index, texcoord_index] : mesh.indices)
		{
			Vertices.push_back(Attrib.vertices[3 * static_cast<size_t>(vertex_index) + 0]);
			Vertices.push_back(Attrib.vertices[3 * static_cast<size_t>(vertex_index) + 1]);
			Vertices.push_back(Attrib.vertices[3 * static_cast<size_t>(vertex_index) + 2]);
			if (texcoord_index >= 0)
			{
				Vertices.push_back(Attrib.texcoords[2 * static_cast<size_t>(texcoord_index) + 0]);
				Vertices.push_back(Attrib.texcoords[2 * static_cast<size_t>(texcoord_index) + 1]);
			}
			else
			{
				Vertices.push_back(0.0f);
				Vertices.push_back(0.0f);
			}
			Indices.push_back(Indices.size());
		}
	}

	setupModel(Model, Vertices, Indices);

	// Load the texture
	Model.Texture = loadTexture(TexturePath);
	// TODO: Load ship and instanced objects with different textures

	return Model;
}

void ModelLoader::setupModel(Model& Model, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices)
{
	glGenVertexArrays(1, &Model.Vao);
	glGenBuffers(1, &Model.Vbo);
	glGenBuffers(1, &Model.Ebo);

	glBindVertexArray(Model.Vao);

	glBindBuffer(GL_ARRAY_BUFFER, Model.Vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Vertices.size() * sizeof(float)), Vertices.data(),
	             GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Model.Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(Indices.size() * sizeof(unsigned int)),
	             Indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

	glBindVertexArray(0);

	Model.IndexCount = static_cast<int>(Indices.size());
}

GLuint ModelLoader::loadTexture(const char* Path)
{
	GLuint TextureId;
	glGenTextures(1, &TextureId);

	int Width, Height, NrComponents;
	unsigned char* Data = stbi_load(Path, &Width, &Height, &NrComponents, 0);
	if (Data)
	{
		GLenum Format = 0;
		if (NrComponents == 1)
			Format = GL_RED;
		else if (NrComponents == 3)
			Format = GL_RGB;
		else if (NrComponents == 4)
			Format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(Format), Width, Height, 0, static_cast<GLint>(Format),
		             GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(Data);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << Path << std::endl;
		stbi_image_free(Data);
	}

	return TextureId;
}
