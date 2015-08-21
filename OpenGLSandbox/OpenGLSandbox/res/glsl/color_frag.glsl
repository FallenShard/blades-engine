#version 450 core

in VsOut
{
    smooth in vec4 color;
} fsIn;

out vec4 color;

void main()
{
    color = fsIn.color;
}