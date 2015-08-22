#version 450 core

layout(quads, fractional_odd_spacing, ccw) in;

in TCS_OUT
{
    vec3 color;
} tesIn[];

out TES_OUT
{
    vec3 position;
    vec3 color;
    vec2 texCoord;
} tesOut;

uniform mat4 MVP;

uniform sampler2D heightMap;
uniform sampler2D detailHeightMap;

layout(std140) uniform TerrainParams
{
    uniform float detailFactor;
    uniform float heightFactor;
    uniform float patches;
    uniform float terrainSize;
};

uniform float worldScale;

void main()
{
    vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 b = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 position = mix(a, b, gl_TessCoord.y);

    float texCoordS = (position.x * worldScale + terrainSize / 2) / terrainSize;
    float texCoordT = (position.z * worldScale + terrainSize / 2) / terrainSize;

    vec2 texCoord = vec2(texCoordS, texCoordT);
    tesOut.texCoord = texCoord;

    float height = texture(heightMap, texCoord).r;
    float detailHeight = texture(detailHeightMap, texCoord * patches / 8.f).r;

    position.y = height * heightFactor + detailHeight * detailFactor * heightFactor;
    tesOut.position = position.xyz;

    vec3 aa = mix(tesIn[0].color, tesIn[1].color, gl_TessCoord.x);
    vec3 bb = mix(tesIn[3].color, tesIn[2].color, gl_TessCoord.x);
    tesOut.color = mix(aa, bb, gl_TessCoord.y);

    gl_Position = MVP * position;
}