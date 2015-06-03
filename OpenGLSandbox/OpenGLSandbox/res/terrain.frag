#version 440 core

out vec4 finalColor;

uniform sampler2D hMap;
uniform sampler2D fMap;
uniform sampler2D dMap;

in TES_OUT
{
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 texCoord;
} fsIn;

//uniform vec4 mvLightDir;
uniform vec3 cameraPos;

uniform int wireframe;

void main()
{
    //vec3 n = normalize(fsIn.normal);

    //float intensity = max(dot(n, mvLightDir.xyz), 0.f);

    //vec3 col = fsIn.color * 0.2f;
    //col += intensity * vec3(0.5f, 0.5f, 0.5f);

    //vec3 N = normalize(cross(dFdy(fsIn.pos), dFdy(fsIn.pos))); // N is the world normal
    //vec3 L = mvLightDir.xyz;

    //float intensity = max(dot(N, L), 0.f);


    vec3 hMapSample = texture(dMap, fsIn.texCoord).rgb;
    //finalColor = vec4(hMapSample, hMapSample, hMapSample, 1.f);
    //finalColor = vec4(hMapSample, 0.f, 0.f, 1.f);
    if (wireframe == 1)
        finalColor = vec4(fsIn.color, 1.f);
    else {
        
        finalColor = vec4(hMapSample, 1.f);
    }
       

    //finalColor = vec4(intensity, 0.f, 0.f, 1.f);

    //finalColor = vec4(fsIn.normal, 1.f);
    //finalColor = vec4(1.f, 0.5f, 0.1f, 1.f);
}