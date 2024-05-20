#pragma once

#include <glew.h>
#include <glfw3.h>
#include <iostream>

class ShaderLoader {
public:
    static GLuint createProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

private:
    ShaderLoader();
    ~ShaderLoader();
    static GLuint createShader(GLenum ShaderType, const char* ShaderName);
    static std::string readShaderFile(const char* Filename);
    static void printErrorDetails(bool isShader, GLuint Id, const char* Name);
};
