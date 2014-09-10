#version 440 core

layout(location = 1) in vec4 vPosition;
layout(location = 2) in vec4 vColor;

smooth out vec4 outColor;

uniform mat4 perspectiveMatrix;

void main()
{
    gl_Position = perspectiveMatrix * vPosition;

    outColor = vColor;
}