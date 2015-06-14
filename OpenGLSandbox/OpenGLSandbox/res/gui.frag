#version 450 core
 
in vec2 texCoord;

uniform sampler2D atlas;

out vec4 finalColor;
 
void main()
{
    finalColor = texture2D(atlas, texCoord);
}