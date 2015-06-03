#version 450
 
in vec2 texCoord;

uniform sampler2D atlas;
uniform vec4 color;

out vec4 finalColor;
 
void main()
{
    vec4 sam = texture2D(atlas, texCoord).r * color;

    finalColor = sam;
}