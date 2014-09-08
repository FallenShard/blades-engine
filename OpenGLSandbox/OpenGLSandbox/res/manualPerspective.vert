#version 440 core

layout(location = 1) in vec4 vPosition;
layout(location = 2) in vec4 vColor;

smooth out vec4 outColor;

uniform vec2 offset;
uniform vec3 eyeOffset;
uniform mat4 perspectiveMatrix;
uniform float frustumScale;


void main()
{
    vec4 cameraPos = vPosition + vec4(offset.xy, 0.0, 0.0);

	gl_Position = perspectiveMatrix * cameraPos;

    outColor = vColor;
}