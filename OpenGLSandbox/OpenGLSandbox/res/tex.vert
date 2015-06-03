#version 450
 
layout(location = 0) in vec4 coord;

uniform mat4 MVP;

out vec2 texCoord;
 
void main(void)
{
    texCoord = coord.zw;
    gl_Position = MVP * vec4(coord.xy, 0, 1);
}