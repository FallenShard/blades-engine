#version 440 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec4 color;

uniform mat4 MVP;

out VsOut
{
    smooth out vec4 color;
} vsOut;

void main()
{
    vsOut.color = color;
    gl_Position = MVP * vec4(position, 1.f);
}
