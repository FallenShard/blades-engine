#version 450 core
 
in vec2 texCoord;

uniform sampler2D texture;
uniform vec2 offset;

out vec4 finalColor;
 
void main()
{
    finalColor = texture2D(texture, texCoord + offset);
}