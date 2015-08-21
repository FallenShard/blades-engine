#version 450 core

in TES_OUT
{
    vec3 position;
    vec3 color;
    vec2 texCoord;
} fsIn;

out vec4 finalColor;

uniform sampler2D heightMap;
uniform sampler2D detailHeightMap;
uniform sampler2D terrainTexture;
uniform sampler2D noiseMap;
uniform sampler2D patchTexture;

uniform mat3 normalMatrix;

uniform vec4 mvLightDir;

layout(std140) uniform TerrainParams
{
    uniform float detailFactor;
    uniform float heightFactor;
    uniform float patches;
    uniform float terrainSize;
};

uniform float worldScale;

uniform int wireframe;

void main()
{
    if (wireframe == 1)
        finalColor = vec4(fsIn.color, 1.f);
    else
    {
        vec3 terrainSample = texture(terrainTexture, fsIn.texCoord).rgb;
        vec3 patchSample = texture(patchTexture, fsIn.texCoord * patches).rgb;
        float noiseSample = texture(noiseMap, fsIn.texCoord).r;
        
        float offset = 1.f / textureSize(terrainTexture, 0).x * worldScale;
        const vec3 off = vec3(-offset, 0, offset);

        float hL = texture(heightMap, fsIn.texCoord + off.xy).r * heightFactor;
                 + texture(detailHeightMap, (fsIn.texCoord + off.xy) * patches).r * heightFactor * detailFactor;
        float hR = texture(heightMap, fsIn.texCoord + off.zy).r * heightFactor;              
                 + texture(detailHeightMap, (fsIn.texCoord + off.zy) * patches).r * heightFactor * detailFactor;
        float hD = texture(heightMap, fsIn.texCoord + off.yz).r * heightFactor;              
                 + texture(detailHeightMap, (fsIn.texCoord + off.yz) * patches).r * heightFactor * detailFactor;
        float hU = texture(heightMap, fsIn.texCoord + off.yx).r * heightFactor;              
                 + texture(detailHeightMap, (fsIn.texCoord + off.yx) * patches).r * heightFactor * detailFactor;
    
        vec3 normal;
        normal.x = hL - hR;
        normal.z = hU - hD;
        normal.y = 2.f;
        normal = normalize(normalMatrix * normal);

        float diffuseCoeff = max(dot(normal, mvLightDir.xyz), 0.f);

        vec3 blend = mix(terrainSample, patchSample, noiseSample);

        finalColor = vec4(blend, 1.f) * 0.2f;
        finalColor += vec4(blend, 1.f) * diffuseCoeff * 0.8f;
    }
}