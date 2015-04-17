#version 440 core

layout(quads, fractional_odd_spacing, ccw) in;

uniform mat4 MVP;
//uniform float size;
uniform sampler2D hMap;


uniform float time;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

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

uniform float patchSize;
uniform float patchesX;

const float MESH_SIZE = patchesX * patchSize;

const float HEIGHT_FACTOR = MESH_SIZE / 8.f;

void main()
{
    //vec4 position = gl_in[0].gl_Position;
    //position.xz += gl_TessCoord.xy * patchSize;

    vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 b = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 position = mix(a, b, gl_TessCoord.y);

    float posX = (position.x + MESH_SIZE / 2) / MESH_SIZE;
    float posZ = (position.z + MESH_SIZE / 2) / MESH_SIZE;

    vec2 texCoord = vec2(posX, posZ);
    tesOut.texCoord = texCoord;
    float sam = texture(hMap, texCoord).r;
    float val = rand(texCoord);
    position.y = sam * HEIGHT_FACTOR;

    const ivec3 off = ivec3(-1, 0, 1);
    //float hL = textureOffset(hMap, texCoord, off.xy).x * HEIGHT_FACTOR;
    //float hR = textureOffset(hMap, texCoord, off.zy).x * HEIGHT_FACTOR;
    //float hD = textureOffset(hMap, texCoord, off.yz).x * HEIGHT_FACTOR;
    //float hU = textureOffset(hMap, texCoord, off.yx).x * HEIGHT_FACTOR;

    //vec3 n;
    //n.x = hR - hL;
    //n.y = hD - hU;
    //n.z = 2.0f;
    tesOut.normal = normalize(vec3(1.f, 0.f, 0.f));

    //if (position.y > MESH_SIZE / 12)
    //    tesOut.color = vec3(1.f, 1.f, 1.f);
    //else if (position.y > MESH_SIZE / 16)
    //    tesOut.color = vec3(0.4f, 0.2f, 0.f);
    //else if (position.y > MESH_SIZE / 32)
    //    tesOut.color = vec3(1.f, 1.f, 0.f);
    //else if (position.y > 0)
    //    tesOut.color = vec3(0.f, 1.f, 0.f);
    //else
    //    tesOut.color = vec3(0.6f, 0.6f, 1.f);

    //if (position.y < 0)
    //    position.y = position.y / 5 * sin(2 * 3.14159286f * (gl_TessCoord.x + time));
    //position.y = (position.x + 128) / 64 * sin((gl_TessCoord.x + time) * 2 * 3.14159286f);

    bool evenCol = gl_PrimitiveID % 2 == 0;
    bool evenRow = (gl_PrimitiveID >> 6) % 2 == 0;

    if (evenCol && evenRow || (!evenCol && !evenRow))
        tesOut.color = vec3(1.f, 0.3f, 0.3f);
    else
        tesOut.color = vec3(0.3f, 0.3f, 1.f);

    gl_Position = MVP * position;

    tesOut.pos = position.xyz;
    
    vec3 aa = mix(tesIn[0].color, tesIn[1].color, gl_TessCoord.x);
    vec3 bb = mix(tesIn[3].color, tesIn[2].color, gl_TessCoord.x);
    tesOut.color = mix(aa, bb, gl_TessCoord.y);
}