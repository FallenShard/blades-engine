#version 440 core

layout(quads, fractional_odd_spacing, ccw) in;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMatrix;

uniform sampler2D hMap;
uniform sampler2D fMap;
uniform sampler2D dMap;

uniform float time;

in TCS_OUT
{
    vec3 color;
} tesIn[];

out TES_OUT
{
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 texCoord;
} tesOut;

//uniform float patchSize;
uniform float patches;
uniform float terrainSize;
uniform float heightFactor;
uniform float worldScale;
uniform float detailFactor;

void main()
{
    vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 b = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 position = mix(a, b, gl_TessCoord.y);

    float posX = (position.x * worldScale + terrainSize / 2) / terrainSize;
    float posZ = (position.z * worldScale + terrainSize / 2) / terrainSize;

    vec2 texCoord = vec2(posX, posZ);
    tesOut.texCoord = texCoord;

    float height = texture(hMap, texCoord).r;
    float detailHeight = texture(fMap, texCoord * patches / 8).r;

    position.y = height * heightFactor + detailHeight * detailFactor * heightFactor;

    const ivec3 off = ivec3(-1, 0, 1);
    float hL = textureOffset(hMap, texCoord, off.xy).r * heightFactor;
    float hR = textureOffset(hMap, texCoord, off.zy).r * heightFactor;
    float hD = textureOffset(hMap, texCoord, off.yz).r * heightFactor;
    float hU = textureOffset(hMap, texCoord, off.yx).r * heightFactor;
    
    vec3 n;
    n.x = hL - hR;
    n.y = 0.0f;
    n.z = hU - hD;
    n = normalize(n);
    tesOut.normal = normalize(n);
    tesOut.pos = position.xyz;

    vec3 aa = mix(tesIn[0].color, tesIn[1].color, gl_TessCoord.x);
    vec3 bb = mix(tesIn[3].color, tesIn[2].color, gl_TessCoord.x);
    tesOut.color = mix(aa, bb, gl_TessCoord.y);

    gl_Position = MVP * position;
}