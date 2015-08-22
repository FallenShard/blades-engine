#version 450 core

layout(vertices = 4) out;

out TCS_OUT
{
    vec3 color;
} tcsOut[];

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

uniform vec2 screenSize;
uniform float worldScale;
uniform float triSize;
//uniform vec4 cameraPos;
//uniform vec4 viewDir;

highp float clipToSSToTess(vec4 clip0, vec4 clip1)
{
    clip0 /= clip0.w;
    clip1 /= clip1.w;
  
    clip0 *= 0.5f; 
    clip1 *= 0.5f;

    clip0 += 0.5f;
    clip1 += 0.5f; 

    clip0.x *= screenSize.x;
    clip0.y *= screenSize.y;
    clip1.x *= screenSize.x;
    clip1.y *= screenSize.y;

    float d = distance(clip0, clip1);

    return clamp(d / triSize, 1, 64);
}

highp float sphereToSSToTess(vec4 pt0, vec4 pt1, float diameter)
{
    vec4 center = 0.5 * (pt0 + pt1);
    vec4 eyePoint0 = MV * center;
    vec4 eyePoint1 = eyePoint0;
    eyePoint1.x += worldScale * diameter;

    vec4 clipPoint0 = P * eyePoint0;
    vec4 clipPoint1 = P * eyePoint1;
    return clipToSSToTess(clipPoint0, clipPoint1);
}



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
    //vec4 center = 0.25 * (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position + gl_in[3].gl_Position);
    float sideLen = max(abs(gl_in[0].gl_Position.x - gl_in[3].gl_Position.x), abs(gl_in[0].gl_Position.x - gl_in[1].gl_Position.x));
    float diagLen = sqrt(2 * sideLen * sideLen);

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
        gl_TessLevelOuter[0] = sphereToSSToTess(gl_in[3].gl_Position, gl_in[0].gl_Position, sideLen);
        gl_TessLevelOuter[1] = sphereToSSToTess(gl_in[0].gl_Position, gl_in[1].gl_Position, sideLen);
        gl_TessLevelOuter[2] = sphereToSSToTess(gl_in[1].gl_Position, gl_in[2].gl_Position, sideLen);
        gl_TessLevelOuter[3] = sphereToSSToTess(gl_in[2].gl_Position, gl_in[3].gl_Position, sideLen);

        gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]);
        gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]);
    //}
    

    vec3 colors[4];
    colors[0] = vec3(1.f, 0.f, 0.f);
    colors[1] = vec3(0.f, 1.f, 0.f);
    colors[2] = vec3(0.f, 0.f, 1.f);
    colors[3] = vec3(1.f, 1.f, 0.f);

    tcsOut[gl_InvocationID].color = colors[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}