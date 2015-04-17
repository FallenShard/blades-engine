#version 440 core

layout(vertices = 4) out;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

float edgeLength(vec3 v1, vec3 v2)
{
    vec3 d = v1 - v2;

    return min(length(d), 32);
}

float edgeTessellation(vec4 v1, vec4 v2)
{
    return 2.f;
}

vec3 getColorFromTess(float tess)
{
    if (tess > 32)
        return vec3(1.f, 0.f, 0.f);
    else if (tess > 16)
        return vec3(0.f, 1.f, 0.f);
    else if (tess > 8)
        return vec3(0.f, 0.f, 1.f);
    else if (tess > 4)
        return vec3(0.f, 1.f, 1.f);
}

highp float ClipToSSTess(vec4 c0, vec4 c1)
{
    c0 /= c0.w;
    c1 /= c1.w;
  
    c0 *= 0.5f; c0 += 0.5f;
    c1 *= 0.5f; c1 += 0.5f;

    c0.x *= 1366;
    c0.y *= 768;
    c1.x *= 1366;
    c1.y *= 768;

    float d = distance(c0, c1);

    return clamp(d / 32.f, 0, 64);
}

float SphereToSSTess(vec3 p0, vec3 p1, float diameter)
{
    vec3 center = 0.5 * (p0 + p1);
    vec4 v0 = MV * vec4(center, 1);
    vec4 v1 = v0;
    v1.x += 1.0 * diameter;

    vec4 c0 = P * v0;
    vec4 c1 = P * v1;
    return ClipToSSTess(c0, c1);
}

out TCS_OUT
{
    vec3 color;
} tcs_out[];

void main()
{
    //vec4 centre = 0.25 * (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position + gl_in[3].gl_Position);
    float sideLen = max(abs(gl_in[1].gl_Position.x - gl_in[0].gl_Position.x), abs(gl_in[1].gl_Position.x - gl_in[2].gl_Position.x));
    float diagLen = sqrt(2*sideLen*sideLen);

    //float tess = SphereToSSTess(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, sideLen);
    gl_TessLevelOuter[1] = SphereToSSTess(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, sideLen);
    gl_TessLevelOuter[2] = SphereToSSTess(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, sideLen);
    gl_TessLevelOuter[3] = SphereToSSTess(gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz, sideLen);
    gl_TessLevelOuter[0] = SphereToSSTess(gl_in[3].gl_Position.xyz, gl_in[0].gl_Position.xyz, sideLen);
    
    gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]);
    gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]);

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    vec3 colors[4];

    colors[0] = vec3(1.f, 0.f, 0.f);
    colors[1] = vec3(1.f, 1.f, 0.f);
    colors[2] = vec3(0.f, 1.f, 1.f);
    colors[3] = vec3(0.f, 0.f, 1.f);

    tcs_out[gl_InvocationID].color = colors[gl_InvocationID];
}