#version 440 core

in vec4 outColor;
out vec4 fColor;


void main()
{
    float val = gl_FragCoord.y / 500.f;
    if (val > 1.f) val = 1.f;
    /*
    fColor = mix(vec4(1.f, 0.f, 0.f, 1.f), vec4(0.f, 1.f, 1.f, 1.f), val);*/
    fColor = outColor;
}