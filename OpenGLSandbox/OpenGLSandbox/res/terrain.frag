#version 440 core

out vec4 finalColor;

uniform sampler2D hMap;

in TES_OUT
{
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 texCoord;
} fsIn;

uniform vec4 mvLightDir;
uniform vec3 cameraPos;

void main()
{
    //vec3 n = normalize(fsIn.normal);

    //float intensity = max(dot(n, mvLightDir.xyz), 0.f);

    //vec3 col = fsIn.color * 0.2f;
    //col += intensity * vec3(0.5f, 0.5f, 0.5f);

    float hMapSample = texture(hMap, fsIn.texCoord).r;
    //finalColor = vec4(hMapSample, hMapSample, hMapSample, 1.f);
    //finalColor = vec4(hMapSample, 0.f, 0.f, 1.f);
    finalColor = vec4(hMapSample, 0.f, 0.f, 1.f);

    //finalColor = vec4(intensity, 0.f, 0.f, 1.f);

    //finalColor = vec4(fsIn.normal, 1.f);
    //finalColor = vec4(1.f, 0.5f, 0.1f, 1.f);
}