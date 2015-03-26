#version 440 core

layout(vertices = 4) out;

uniform vec4 cameraPos;

void setTess(float outer, float inner)
{
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = outer;
        gl_TessLevelOuter[1] = outer;
        gl_TessLevelOuter[2] = outer;
        gl_TessLevelOuter[3] = outer;
        gl_TessLevelInner[0] = inner;
        gl_TessLevelInner[1] = inner;
    }
}

float calcTessLevel(float dist0, float dist1, float dist2, float dist3)
{
    float avg = (dist0 + dist1 + dist2 + dist3) / 4;

    if (avg <= 300.0) {
        return 2.0;
    }
    else if (avg <= 600.0) {
        return 2.0;
    }
    else if (avg <= 1000.0) {
        return 2.0;
    }
    else
        return 2.0;
}

void main()
{
    float dist0 = distance(cameraPos, gl_in[0].gl_Position);
    float dist1 = distance(cameraPos, gl_in[1].gl_Position);
    float dist2 = distance(cameraPos, gl_in[2].gl_Position);
    float dist3 = distance(cameraPos, gl_in[3].gl_Position);

    float tess = calcTessLevel(dist0, dist1, dist2, dist3);

    setTess(tess * 2, tess);

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}