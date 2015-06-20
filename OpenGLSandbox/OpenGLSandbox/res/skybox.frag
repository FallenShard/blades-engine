#version 450 core
 
in vec3 fsTexCoord;

uniform sampler2DArray skybox;

out vec4 finalColor;
 
void main()
{
    finalColor = texture(skybox, fsTexCoord);
}