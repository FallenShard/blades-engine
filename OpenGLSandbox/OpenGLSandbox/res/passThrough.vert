#version 440 core

layout(location = 0) in vec2 position;

smooth out vec2 UV;

void main()
{
    UV = (position.xy + vec2(1.f, 1.f)) / 2.f;
    gl_Position = vec4(position.x, position.y, 0.f, 1.f);
}