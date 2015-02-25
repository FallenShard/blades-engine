#version 440 core

layout(quads, equal_spacing, ccw) in;

uniform mat4 MVP;
//uniform float size;
uniform sampler2D hMap;


uniform float time;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

out TES_OUT
{
    vec3 pos;
} tesOut;

void main()
{
    vec4 position = gl_in[0].gl_Position;
    position.xz += gl_TessCoord.xy * 8.f - 4.f;

    //position.y = sin(time * 2 * 3.14159286f * gl_TessCoord.x);
    position.y = (position.x + 128) / 64 * sin((gl_TessCoord.x + time) * 2 * 3.14159286f);

    //vec4 a = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, gl_TessCoord.x);
    //vec4 b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
    //vec4 position = mix(a, b, gl_TessCoord.y);

    gl_Position = MVP * position;

    tesOut.pos = position.xyz;
}