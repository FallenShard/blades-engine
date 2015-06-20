#version 440 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 texCoord;

uniform mat4 MVP;

out vec3 fsTexCoord;

void main()
{
    fsTexCoord = texCoord;
    gl_Position = MVP * vec4(position, 1.f);
}
