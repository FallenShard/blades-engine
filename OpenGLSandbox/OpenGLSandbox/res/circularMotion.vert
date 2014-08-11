#version 440 core

layout (location = 1) in vec4 vPosition;
layout (location = 2) in vec4 vColor;

flat out vec4 outColor;

//uniform vec2 offset;
uniform float loopDuration = 4.f;
uniform float time;

void main()
{
	//gl_Position = vPosition + vec4(offset.x, offset.y, 0.0, 0.0);
	float scale = 3.141592 * 2 / loopDuration;

	gl_Position = vPosition + vec4(cos(scale * time) * 0.5f, sin(scale * time) * 0.5f, 0.f, 0.f);

	outColor = vColor;
}