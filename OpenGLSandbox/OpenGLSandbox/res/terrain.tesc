#version 450 core

layout(vertices = 4) out;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

uniform float triSize;
uniform vec2 screenSize;
//uniform vec4 cameraPos;
//uniform vec4 viewDir;

uniform float worldScale;

float edgeLength(vec3 v1, vec3 v2)
{
    vec3 d = v1 - v2;

    return min(length(d), 32);
}

highp float ClipToSSTess(vec4 c0, vec4 c1)
{
    c0 /= c0.w;
    c1 /= c1.w;
  
    c0 *= 0.5f; c0 += 0.5f;
    c1 *= 0.5f; c1 += 0.5f;

    c0.x *= screenSize.x;
    c0.y *= screenSize.y;
    c1.x *= screenSize.x;
    c1.y *= screenSize.y;

    float d = distance(c0, c1);

    return clamp(d / triSize, 1, 64);
}

highp float SphereToSSTess(vec4 p0, vec4 p1, float diameter)
{
    vec4 center = 0.5 * (p0 + p1);
    vec4 v0 = MV * center;
    vec4 v1 = v0;
    v1.x += worldScale * diameter;

    vec4 c0 = P * v0;
    vec4 c1 = P * v1;
    return ClipToSSTess(c0, c1);
}

out TCS_OUT
{
    vec3 color;
} tcs_out[];

bool inFrustum(const vec3 pt, const vec3 cameraPos, const vec3 viewDir, float margin)
{
    // conservative frustum culling
    //vec3 camToPt = pt - cameraPos;
    //vec3 ptToCameraDir = viewDir * dot(camToPt, viewDir) - camToPt;
    //vec3 patchRealigned = pt + normalize(ptToCameraDir) * min(margin, length(ptToCameraDir));
    //vec4 patchScreenspaceCenter = MV * vec4(patchRealigned, 1.0);

    //if(((patchScreenspaceCenter.x/patchScreenspaceCenter.w > -1.0) && (patchScreenspaceCenter.x/patchScreenspaceCenter.w < 1.0) &&
    //    (patchScreenspaceCenter.y/patchScreenspaceCenter.w > -1.0) && (patchScreenspaceCenter.y/patchScreenspaceCenter.w < 1.0) &&
    //    (patchScreenspaceCenter.w>0)) || (length(pt - cameraPos) < margin))
    //{
    //    return true;
    //}


    return true;
}

void main()
{
    vec4 center = 0.25 * (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position + gl_in[3].gl_Position);
    float sideLen = max(abs(gl_in[0].gl_Position.x - gl_in[3].gl_Position.x), abs(gl_in[0].gl_Position.x - gl_in[1].gl_Position.x));
    float diagLen = sqrt(2*sideLen*sideLen);

    //if (!inFrustum(center.xyz, cameraPos.xyz / worldScale, viewDir.xyz, diagLen))
    //{
    //    gl_TessLevelOuter[0] = 0.f;
    //    gl_TessLevelOuter[1] = 0.f;
    //    gl_TessLevelOuter[2] = 0.f;
    //    gl_TessLevelOuter[3] = 0.f;
    //    gl_TessLevelInner[0] = 0.f;
    //    gl_TessLevelInner[1] = 0.f;
    //}
    //else
    //{
        gl_TessLevelOuter[0] = SphereToSSTess(gl_in[3].gl_Position, gl_in[0].gl_Position, sideLen);
        gl_TessLevelOuter[1] = SphereToSSTess(gl_in[0].gl_Position, gl_in[1].gl_Position, sideLen);
        gl_TessLevelOuter[2] = SphereToSSTess(gl_in[1].gl_Position, gl_in[2].gl_Position, sideLen);
        gl_TessLevelOuter[3] = SphereToSSTess(gl_in[2].gl_Position, gl_in[3].gl_Position, sideLen);

        gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]);
        gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]);
    //}
    

    vec3 colors[4];
    colors[0] = vec3(1.f, 0.f, 0.f);
    colors[1] = vec3(0.f, 1.f, 0.f);
    colors[2] = vec3(0.f, 0.f, 1.f);
    colors[3] = vec3(1.f, 1.f, 0.f);

    tcs_out[gl_InvocationID].color = colors[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}