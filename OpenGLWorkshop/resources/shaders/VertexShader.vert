#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 mvp;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    TexCoord = texCoord;
    Normal = normal;
    gl_Position = mvp * vec4(position, 1.0);
}
