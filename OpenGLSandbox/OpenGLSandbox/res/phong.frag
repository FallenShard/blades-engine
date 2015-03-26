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

out vec4 color;

const float k0 = 1.f;
const float k1 = 0.01f;
const float k2 = 0.02f;

void main()
{
    vec3 L = mvLightPos.xyz - fsIn.position;
    float d = length(L);
    L = normalize(L);

    vec3 N = normalize(fsIn.normal);
    
    

    float attenuation = 1.f / (k0 + k1 * d + k2 * d * d);

    float diffuse = max(0, dot(fsIn.normal, L));
    float specular = 0.f;

    if (diffuse > 0.f)
    {
        vec3 V = normalize(-fsIn.position);
        vec3 H = normalize(L + V);
        float specAngle = max(dot(N, H), 0.0);
        specular = pow(specAngle, material.shininess);
    }

    vec4 ambCol = material.ambient * 0.15f;
    vec4 diffCol = material.diffuse * diffuse * attenuation * 0.25f;
    vec4 specCol = material.specular * specular * attenuation;

    color = ambCol + diffCol + specCol;
}