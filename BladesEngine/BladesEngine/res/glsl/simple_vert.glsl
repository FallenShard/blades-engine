#version 450 core

layout(location = 0) in vec2 position;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 0.f, 1.f);
}
