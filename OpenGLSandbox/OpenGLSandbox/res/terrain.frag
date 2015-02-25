#version 440 core

out vec4 finalColor;

in TES_OUT
{
    vec3 pos;
} fsIn;

void main()
{
    float x = fsIn.pos.x;
    float y = fsIn.pos.y;
    float z = fsIn.pos.z;

    x /= 128.f;
    //y = (y + 8.f) / 16.f;
    y /= 16.f;
    z /= 128.f;

    finalColor = vec4(1 + y, 1.0 - y, 0.f, 1.f);
}