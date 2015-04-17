#version 440 core

layout(location = 0) in vec3 position;

uniform sampler2D hMap;

uniform float patchSize;
uniform float patchesX;

const float MESH_SIZE = patchesX * patchSize;

const float HEIGHT_FACTOR = MESH_SIZE / 8.f;

void main()
{
    float posX = (position.x + MESH_SIZE / 2) / MESH_SIZE;
    float posZ = (position.z + MESH_SIZE / 2) / MESH_SIZE;

    vec2 texCoord = vec2(posX, posZ);
    float height = texture(hMap, texCoord).r * HEIGHT_FACTOR;

    gl_Position = vec4(position, 1.f);
    gl_Position.y = height;
}
