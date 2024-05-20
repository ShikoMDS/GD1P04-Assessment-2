#ifndef MODELLOADER_H
#define MODELLOADER_H
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glew.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

struct Model {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;
    int indexCount;
};

class ModelLoader {
public:
    Model loadModel(const char* path);
private:
    void setupModel(Model& model, const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    GLuint loadTexture(const char* path);
};

#endif // MODELLOADER_H
