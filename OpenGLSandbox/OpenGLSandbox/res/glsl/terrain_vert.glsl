#version 450 core

layout(location = 0) in vec3 position;

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
    float texCoordS = (position.x * worldScale + terrainSize / 2) / terrainSize;
    float texCoordT = (position.z * worldScale + terrainSize / 2) / terrainSize;

    vec2 texCoord = vec2(texCoordS, texCoordT);
    float height = texture(heightMap, texCoord * patches).r * heightFactor;
    float detailHeight = texture(detailHeightMap, texCoord).r * heightFactor * detailFactor;

    gl_Position = vec4(position, 1.f);
    gl_Position.y = height + detailHeight;
}
