#version 460 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}
