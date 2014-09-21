#version 440 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
`
smooth out vec4 outColor;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;

void main()
{
	vec4 cameraPos = modelToWorldMatrix * vPosition;
	cameraPos = worldToCameraMatrix * cameraPos;
	gl_Position = cameraToClipMatrix * cameraPos;
	outColor = vColor;
}
