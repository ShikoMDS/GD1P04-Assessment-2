#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 instanceMatrix;

uniform mat4 viewProjectionMatrix;

out vec2 TexCoord;

void main() {
    gl_Position = viewProjectionMatrix * instanceMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
