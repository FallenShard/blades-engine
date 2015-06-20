#version 440 core

out vec4 finalColor;

uniform sampler2D hMap;
uniform sampler2D fMap;
uniform sampler2D dMap;
uniform sampler2D nMap;
uniform sampler2D detMap;

in TES_OUT
{
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 texCoord;
} fsIn;

uniform vec4 mvLightDir;

uniform mat4 MV;
uniform mat3 normalMatrix;

uniform int wireframe;

uniform float patches;
uniform float heightFactor;
uniform float terrainSize;
uniform float worldScale;
uniform float detailFactor;

void main()
{
    if (wireframe == 1)
        finalColor = vec4(fsIn.color, 1.f);
    else
    {
        vec3 hMapSample = texture(dMap, fsIn.texCoord).rgb;
        vec3 fMapSample = texture(fMap, fsIn.texCoord).rgb;
        vec3 detMapSample = texture(detMap, fsIn.texCoord * patches).rgb;
        float noiseSample = texture(nMap, fsIn.texCoord).r;
        
        float offset = 1.f / textureSize(hMap, 0).x * worldScale;
        const vec3 off = vec3(-offset, 0, offset);

        float hL = texture(hMap, fsIn.texCoord + off.xy).r * heightFactor;
                 + texture(dMap, (fsIn.texCoord + off.xy) * patches).r * heightFactor * detailFactor;
        float hR = texture(hMap, fsIn.texCoord + off.zy).r * heightFactor;              
                 + texture(dMap, (fsIn.texCoord + off.zy) * patches).r * heightFactor * detailFactor;
        float hD = texture(hMap, fsIn.texCoord + off.yz).r * heightFactor;              
                 + texture(dMap, (fsIn.texCoord + off.yz) * patches).r * heightFactor * detailFactor;
        float hU = texture(hMap, fsIn.texCoord + off.yx).r * heightFactor;              
                 + texture(dMap, (fsIn.texCoord + off.yx) * patches).r * heightFactor * detailFactor;
    
        vec3 n;
        n.x = hL - hR;
        n.z = hU - hD;
        n.y = 2.f;
        n = normalize(n);
        n = normalize(normalMatrix * n);

        //vec3 eyeLightDir = normalize(vec3(MV * normalize(vec4(1.f, 1.0f, 1.f, 0.f))));
        
        float diffuse = max(dot(n, mvLightDir.xyz), 0.f);

        vec3 blend = mix(hMapSample, detMapSample, noiseSample);
        finalColor = vec4(blend, 1.f) * 0.2f;
        finalColor += vec4(blend, 1.f) * diffuse * 0.8f;
    }
}