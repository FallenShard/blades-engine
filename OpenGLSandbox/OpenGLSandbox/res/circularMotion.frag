#version 440 core

out vec4 fColor;

const vec4 firstColor = vec4(1.f, 0.f, 0.f, 1.f);
const vec4 secondColor = vec4(0.f, 1.f, 0.f, 1.f);

uniform float fragLoopDuration = 4.f;
uniform float time;
uniform float screenHeight;

void main()
{
    float scale = 3.141592f * 2 / fragLoopDuration;

    float lerpVal = (cos(scale * time) + 1) / 2;
    fColor = mix(firstColor, secondColor, lerpVal) * gl_FragCoord.y / screenHeight;
}