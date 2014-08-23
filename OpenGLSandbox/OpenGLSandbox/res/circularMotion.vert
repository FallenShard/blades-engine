#version 440 core

layout (location = 1) in vec4 vPosition;
layout (location = 2) in vec4 vColor;

smooth out vec4 outColor;

uniform float loopDuration = 4.f;
uniform float time;

void main()
{
	float scale = 3.141592f * 2 / loopDuration;

	vec4 totalOffset = vec4(cos(scale * time) * 0.5f, sin(scale * time) * 0.5f, 0.f, 0.f);

	gl_Position = vPosition + totalOffset;

	outColor = vColor;
}