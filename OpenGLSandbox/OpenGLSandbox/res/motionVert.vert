#version 440 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

smooth out vec4 outColor;

uniform mat4 cameraToClipMatrix;
uniform mat4 modelToCameraMatrix;

void main()
{
	vec4 cameraPos = modelToCameraMatrix * vPosition;
	gl_Position = cameraToClipMatrix * cameraPos;
	outColor = vColor;
}
