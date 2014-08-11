#version 440 core

in vec4 outColor;
out vec4 fColor;


const vec4 firstColor = vec4(1.f, 0.f, 0.f, 1.f);
const vec4 secondColor = vec4(0.f, 1.f, 0.f, 1.f);

uniform float loopDuration = 4.f;
uniform float time;

void main()
{
    float lerpVal = time / loopDuration;
    fColor = mix(firstColor, secondColor, lerpVal);
}