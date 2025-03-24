cbuffer VSConstBuffer1 : register(b0)
{
    matrix mxViewProj;
};

cbuffer VSConstBuffer2 : register(b1)
{
    float4 color;
};

struct VSOut
{
    float4 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.color = color;
    vso.pos = mul(float4(pos, 1.0f), mxViewProj);
    return vso;
}