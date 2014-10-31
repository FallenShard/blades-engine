#version 440 core

layout(location = 1) in vec2 vPosition;

smooth out vec2 UV;

void main()
{
    UV = (vPosition.xy + vec2(1.f, 1.f)) / 2.f;
    gl_Position = vec4(vPosition.x, vPosition.y, 0.f, 1.f);
}