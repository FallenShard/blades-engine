struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUT VS(float4 pos : POSITION, float4 col : COLOR)
{
    VS_OUT output;
    output.position = pos;
    output.color = col;
    return output;
}