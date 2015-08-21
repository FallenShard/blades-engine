#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMatrix;

out VsOut
{
    smooth out vec3 position;
    smooth out vec3 normal;
} vsOut;

void main()
{
    vsOut.position = (MV * vec4(position, 1.f)).xyz;
    vsOut.normal   = normalMatrix * normal;

    gl_Position = MVP * vec4(position, 1.f);
}
