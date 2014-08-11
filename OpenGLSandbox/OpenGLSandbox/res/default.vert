#version 440 core

layout(location = 1) in vec4 vPosition;
layout(location = 2) in vec4 vColor;

flat out vec4 outColor;

void main()
{
    gl_Position = vPosition;
    outColor = vColor;
}