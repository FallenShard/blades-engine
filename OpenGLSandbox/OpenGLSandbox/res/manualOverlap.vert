#version 440 core

layout(location = 1) in vec4 vPosition;
layout(location = 2) in vec4 vColor;

smooth out vec4 outColor;

uniform mat4 perspectiveMatrix;
uniform vec3 offset;


void main()
{
    vec4 cameraPos = vPosition + vec4(offset.x, offset.y, offset.z, 0.0);

    gl_Position = perspectiveMatrix * cameraPos;
    outColor = vColor;
}