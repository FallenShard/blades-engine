#version 440 core

/* Courtesy of NVIDIA's Timothy Lottes */

in vec2 UV;

uniform sampler2D renderedTexture;

out vec4 color;

uniform float screenWidth;
uniform float screenHeight;
 
void main()
{
    float FXAA_SPAN_MAX = 8.f;
    float FXAA_REDUCE_MUL = 1.0/FXAA_SPAN_MAX;
    float FXAA_REDUCE_MIN = (1.0/128.0);
    vec2 texcoordOffset = vec2(1.f/screenWidth, 1.f/screenHeight);
    vec3 rgbNW = texture2D(renderedTexture, UV.xy + (vec2(-1.0, -1.0) * texcoordOffset)).xyz;
    vec3 rgbNE = texture2D(renderedTexture, UV.xy + (vec2(+1.0, -1.0) * texcoordOffset)).xyz;
    vec3 rgbSW = texture2D(renderedTexture, UV.xy + (vec2(-1.0, +1.0) * texcoordOffset)).xyz;
    vec3 rgbSE = texture2D(renderedTexture, UV.xy + (vec2(+1.0, +1.0) * texcoordOffset)).xyz;
    vec3 rgbM  = texture2D(renderedTexture, UV.xy).xyz;
    
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
      
    float rcpDirMin = 1.f / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), 
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * texcoordOffset;
        
    vec3 rgbA = (1.0/2.0) * (
    texture2D(renderedTexture, UV.xy + dir * (1.0/3.0 - 0.5)).xyz +
    texture2D(renderedTexture, UV.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
    texture2D(renderedTexture, UV.xy + dir * (0.0/3.0 - 0.5)).xyz +
    texture2D(renderedTexture, UV.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma);

    if ((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        color.xyz = rgbA;
    } 
    else
    {
        color.xyz = rgbB;
    }

    color.a = 1.0;
}