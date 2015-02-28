#version 440 core

in VsOut
{
    smooth in vec3 position;
    smooth in vec3 normal;
} fsIn;

layout(std140) uniform Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

uniform vec4 mvLightPos;
uniform vec3 mvCameraPos;

out vec4 color;

const float k0 = 1.f;
const float k1 = 0.001f;
const float k2 = 0.002f;

void main()
{
    vec3 L = mvLightPos.xyz - fsIn.position;
    float d = length(L);
    L = normalize(L);

    vec3 N = normalize(fsIn.normal);
    vec3 V = normalize(mvCameraPos - fsIn.position);
    vec3 H = normalize(L + V);

    float attenuation = 1.f / (k0 + k1 * d + k2 * d * d);

    float diffuse = max(0, dot(fsIn.normal, L));
    float specular = max(0, pow(dot(N, H), material.shininess));

    vec4 ambCol = material.ambient * 0.15f;
    vec4 diffCol = material.diffuse * diffuse * attenuation * 0.25;
    vec4 specCol = material.specular * specular * attenuation;

    color = ambCol + diffCol + specCol;
}