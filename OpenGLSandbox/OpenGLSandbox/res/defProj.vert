#version 440 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

smooth out vec4 outColor;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vPosition;
	outColor = vColor;
}
