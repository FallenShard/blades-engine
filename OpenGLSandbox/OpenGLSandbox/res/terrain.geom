#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in TES_OUT
{
    vec3 pos;
    vec3 color;
} vertices[];

out GEOM_OUT
{
    vec3 pos;
    vec3 color;
    vec3 normal;
} fragIn;   

uniform mat3 normalMatrix; 

void main()
{
    int i;

    vec3 diff1 = vertices[1].pos - vertices[0].pos;
    vec3 diff2 = vertices[2].pos - vertices[0].pos;

    vec3 normal = normalMatrix * normalize(cross(diff2, diff1));

    for (i = 0; i < gl_in.length(); i++)
    {
        fragIn.pos = vertices[i].pos;
        fragIn.color = vertices[i].color;
        fragIn.normal = normal;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}