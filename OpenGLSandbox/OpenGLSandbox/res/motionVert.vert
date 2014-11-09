#version 440 core

layout(location = 0) in vec4 vPosition;
//layout(location = 1) in vec4 vColor;

smooth out vec4 outColor;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform vec4 color;

void main()
{
    mat4 MV = worldToCameraMatrix * modelToWorldMatrix;
    // In world coordinates
    vec3 lightPos = vec3(0.f, 0.f, -5.f);

    // Diffuse lighting

    vec3 normal = normalize(vPosition.xyz);

    vec3 modelViewNormal = vec3(MV * vec4(normal, 0.0));
    vec3 modelViewVertex = vec3(MV * vPosition);
    vec3 modelViewLight = vec3(worldToCameraMatrix * vec4(lightPos, 1.f));

    float dist = length(modelViewLight - modelViewVertex);

    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(modelViewLight - modelViewVertex);

    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(modelViewNormal, lightVector), 0.0);

    diffuse = diffuse * (1.0 / (1.0 + (0.05 * dist * dist)));

    outColor = vec4(1.f, 0.f, 0.f, 1.f) * diffuse * 0.9;

    gl_Position = cameraToClipMatrix * MV * vPosition;
}
