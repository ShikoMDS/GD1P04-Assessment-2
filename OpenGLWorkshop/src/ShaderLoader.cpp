/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : ShaderLoader.cpp
Description : Implementations for shader loading through vertex and
			  fragment text files following OpenGL rendering pipeline
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "ShaderLoader.h"

ShaderLoader::ShaderLoader() = default;

ShaderLoader::~ShaderLoader() = default;

GLuint ShaderLoader::createProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename)
{
	// Create shaders using .frag and .vert external files
	const GLuint VertexShaderId = createShader(GL_VERTEX_SHADER, VertexShaderFilename);
	const GLuint FragmentShaderId = createShader(GL_FRAGMENT_SHADER, FragmentShaderFilename);

	// Create program handle and attach and link shaders
	const GLuint Program = glCreateProgram();
	glAttachShader(Program, VertexShaderId);
	glAttachShader(Program, FragmentShaderId);
	glLinkProgram(Program);

	// Check for link errors
	int LinkResult = 0;
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkResult);
	if (LinkResult == GL_FALSE)
	{
		const std::string ProgramName = std::string(VertexShaderFilename) + std::string(FragmentShaderFilename);
		printErrorDetails(false, Program, ProgramName.c_str());
		return 0;
	}

	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	return Program;
}

GLuint ShaderLoader::createShader(const GLenum ShaderType, const char* ShaderName)
{
	// Read the shader files and save the source code as strings
	const std::string ShaderSourceCode = readShaderFile(ShaderName);

	// Create the shader ID and create pointers for source code string and length
	const GLuint ShaderId = glCreateShader(ShaderType);
	const char* ShaderCodePtr = ShaderSourceCode.c_str();
	const int ShaderCodeSize = static_cast<int>(ShaderSourceCode.size());

	// Populate the Shader Object (ID) and compile
	glShaderSource(ShaderId, 1, &ShaderCodePtr, &ShaderCodeSize);
	glCompileShader(ShaderId);

	// Check for errors
	int CompileResult = 0;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &CompileResult);
	if (CompileResult == GL_FALSE)
	{
		printErrorDetails(true, ShaderId, ShaderName);
		return 0;
	}
	return ShaderId;
}

std::string ShaderLoader::readShaderFile(const char* Filename)
{
	// Open file for reading
	std::ifstream File(Filename, std::ios::in);
	std::string ShaderCode;

	// Ensure file is open and readable
	if (!File.good())
	{
		std::cout << "Cannot read file:  " << Filename << std::endl;
		return "";
	}

	// Determine the size of the file in characters and resize the string variable to accommodate
	File.seekg(0, std::ios::end);
	ShaderCode.resize(File.tellg());

	// Set the position of the next character to be read back to the beginning
	File.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	File.read(ShaderCode.data(), ShaderCode.size());
	File.close();
	return ShaderCode;
}

void ShaderLoader::printErrorDetails(const bool isShader, const GLuint Id, const char* Name)
{
	int InfoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true)
		? glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLength)
		: glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> Log(InfoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true)
		? glGetShaderInfoLog(Id, InfoLogLength, nullptr, Log.data())
		: glGetProgramInfoLog(Id, InfoLogLength, nullptr, Log.data());
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << Name << std::endl;
	std::cout << Log.data() << std::endl;
}
