#version 440 core

layout(location = 0) in vec3 position;

uniform sampler2D hMap;
uniform sampler2D fMap;

uniform float patches;
uniform float terrainSize;
uniform float heightFactor;
uniform float detailFactor;
uniform float worldScale;

void main()
{
    float posX = (position.x * worldScale + terrainSize / 2) / terrainSize;
    float posZ = (position.z * worldScale + terrainSize / 2) / terrainSize;

    vec2 texCoord = vec2(posX, posZ);
    float height = texture(hMap, texCoord * patches).r * heightFactor;
    float detailHeight = texture(fMap, texCoord).r * heightFactor * detailFactor;

    gl_Position = vec4(position, 1.f);
    gl_Position.y = height + detailHeight;
}
